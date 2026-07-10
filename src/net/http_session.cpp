#include "web_server/http_session.h"

#include "web_server/constants.h"
#include "web_server/crud/items.h"
#include "web_server/error.h"
#include "web_server/request_context.h"

#include <boost/asio/redirect_error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include <charconv>
#include <chrono>
#include <exception>
#include <string_view>

namespace web_server {
namespace {

namespace beast = boost::beast;
namespace http = beast::http;

struct ParsedCrudTarget {
    bool matched{};
    bool has_id{};
    std::int64_t id{};
};

[[nodiscard]] ParsedCrudTarget parse_crud_target(std::string_view target) {
    ParsedCrudTarget result;

    if (!target.starts_with(crud_items_path)) {
        return result;
    }

    result.matched = true;

    if (target.size() == crud_items_path.size()) {
        return result;
    }

    if (target[crud_items_path.size()] != '/') {
        return {};
    }

    auto const id_part = target.substr(crud_items_path.size() + 1);
    if (id_part.empty() || id_part.find('/') != std::string_view::npos) {
        return {};
    }

    std::int64_t id{};
    auto const [ptr, ec] = std::from_chars(id_part.data(), id_part.data() + id_part.size(), id);
    if (ec != std::errc{} || ptr != id_part.data() + id_part.size()) {
        return {};
    }

    result.has_id = true;
    result.id = id;
    return result;
}

boost::asio::awaitable<http::message_generator>
dispatch_crud_request(RequestContext &ctx, SharedState &state, ParsedCrudTarget const &parsed) {
    auto const method = ctx.method();

    if (!parsed.has_id) {
        if (method == http::verb::get) {
            co_return co_await handle_list_items(ctx, state);
        }
        if (method == http::verb::post) {
            co_return co_await handle_create_item(ctx, state);
        }
        co_return ctx.response().method_not_allowed();
    }

    if (method == http::verb::get) {
        co_return co_await handle_get_item(ctx, state, parsed.id);
    }
    if (method == http::verb::put) {
        co_return co_await handle_update_item(ctx, state, parsed.id);
    }
    if (method == http::verb::delete_) {
        co_return co_await handle_delete_item(ctx, state, parsed.id);
    }

    co_return ctx.response().method_not_allowed();
}

boost::asio::awaitable<http::message_generator>
handle_http_request(http::request<http::string_body> &&request, SharedState &state) {
    RequestContext ctx{std::move(request)};

    try {
        auto const parsed = parse_crud_target(ctx.target());
        if (!parsed.matched) {
            co_return ctx.response().not_found_text();
        }
        co_return co_await dispatch_crud_request(ctx, state, parsed);
    } catch (std::exception const &err) {
        co_return ctx.response().internal_server_error(err.what());
    }
}

} // namespace

boost::asio::awaitable<void> run_http_session(boost::asio::ip::tcp::socket &&socket,
                                              boost::asio::ssl::context &ssl_ctx,
                                              std::shared_ptr<SharedState> state) {
    boost::system::error_code ec;
    beast::ssl_stream<beast::tcp_stream> stream{std::move(socket), ssl_ctx};
    beast::flat_buffer buffer;

    beast::get_lowest_layer(stream).expires_after(io_timeout);
    co_await stream.async_handshake(boost::asio::ssl::stream_base::server,
                                    boost::asio::redirect_error(ec));
    if (ec) {
        log_error(ec, "handshake");
        co_return;
    }

    while (true) {
        http::request_parser<http::string_body> parser;
        parser.body_limit(max_request_body_size);

        beast::get_lowest_layer(stream).expires_after(io_timeout);
        co_await http::async_read(stream, buffer, parser, boost::asio::redirect_error(ec));

        if (ec == http::error::end_of_stream) {
            beast::get_lowest_layer(stream).socket().shutdown(
                boost::asio::ip::tcp::socket::shutdown_send, ec);
            co_return;
        }
        if (ec) {
            log_error(ec, "read");
            co_return;
        }

        http::message_generator message = co_await handle_http_request(parser.release(), *state);
        bool const keep_alive = message.keep_alive();

        co_await beast::async_write(stream, std::move(message), boost::asio::redirect_error(ec));
        if (ec) {
            log_error(ec, "write");
            co_return;
        }

        if (!keep_alive) {
            beast::get_lowest_layer(stream).socket().shutdown(
                boost::asio::ip::tcp::socket::shutdown_send, ec);
            co_return;
        }
    }
}

} // namespace web_server
