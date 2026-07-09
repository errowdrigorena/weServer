#pragma once

#include "web_server/shared_state.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>

#include <memory>

namespace web_server {

[[nodiscard]] boost::asio::awaitable<void> run_server(
    boost::asio::ip::tcp::endpoint listening_endpoint,
    boost::asio::ssl::context& ssl_ctx,
    std::shared_ptr<SharedState> state);

}  // namespace web_server

