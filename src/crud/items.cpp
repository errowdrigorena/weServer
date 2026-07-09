#include "web_server/crud/items.h"

#include <boost/json/array.hpp>
#include <boost/json/serialize.hpp>

#include <string>
#include <vector>

namespace web_server {
namespace {

[[nodiscard]] std::string items_to_json(std::vector<Item> const& items)
{
    boost::json::array array;
    array.reserve(items.size());
    for (auto const& item : items) {
        array.push_back(item.to_json());
    }
    return boost::json::serialize(array);
}

}  // namespace

boost::asio::awaitable<boost::beast::http::message_generator>
handle_list_items(RequestContext& ctx, SharedState& state)
{
    co_return ctx.response().json_response(items_to_json(state.store.list()));
}

boost::asio::awaitable<boost::beast::http::message_generator>
handle_create_item(RequestContext& ctx, SharedState& state)
{
    auto parsed = item_from_json(ctx.body());
    if (!parsed) {
        co_return ctx.response().bad_request_text(parsed.error().message());
    }

    auto created = state.store.create(std::move(*parsed));
    co_return ctx.response().json_response(boost::json::serialize(created.to_json()));
}

boost::asio::awaitable<boost::beast::http::message_generator>
handle_get_item(RequestContext& ctx, SharedState& state, std::int64_t id)
{
    auto item = state.store.get(id);
    if (!item) {
        co_return ctx.response().not_found_text();
    }
    co_return ctx.response().json_response(boost::json::serialize(item->to_json()));
}

boost::asio::awaitable<boost::beast::http::message_generator>
handle_update_item(RequestContext& ctx, SharedState& state, std::int64_t id)
{
    auto parsed = item_from_json(ctx.body());
    if (!parsed) {
        co_return ctx.response().bad_request_text(parsed.error().message());
    }

    auto updated = state.store.update(id, std::move(*parsed));
    if (!updated) {
        co_return ctx.response().not_found_text();
    }
    co_return ctx.response().json_response(boost::json::serialize(updated->to_json()));
}

boost::asio::awaitable<boost::beast::http::message_generator>
handle_delete_item(RequestContext& ctx, SharedState& state, std::int64_t id)
{
    if (!state.store.erase(id)) {
        co_return ctx.response().not_found_text();
    }
    co_return ctx.response().empty_response();
}

}  // namespace web_server
