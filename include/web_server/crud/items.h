#pragma once

#include "web_server/request_context.h"
#include "web_server/shared_state.h"

#include <boost/asio/awaitable.hpp>
#include <boost/beast/http/message_generator.hpp>

#include <cstdint>

namespace web_server {

[[nodiscard]] boost::asio::awaitable<boost::beast::http::message_generator>
handle_list_items(RequestContext& ctx, SharedState& state);

[[nodiscard]] boost::asio::awaitable<boost::beast::http::message_generator>
handle_create_item(RequestContext& ctx, SharedState& state);

[[nodiscard]] boost::asio::awaitable<boost::beast::http::message_generator>
handle_get_item(RequestContext& ctx, SharedState& state, std::int64_t id);

[[nodiscard]] boost::asio::awaitable<boost::beast::http::message_generator>
handle_update_item(RequestContext& ctx, SharedState& state, std::int64_t id);

[[nodiscard]] boost::asio::awaitable<boost::beast::http::message_generator>
handle_delete_item(RequestContext& ctx, SharedState& state, std::int64_t id);

}  // namespace web_server
