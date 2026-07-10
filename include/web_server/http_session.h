#pragma once

#include "web_server/shared_state.h"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>

#include <memory>

namespace web_server {

[[nodiscard]] boost::asio::awaitable<void> run_http_session(boost::asio::ip::tcp::socket &&socket,
                                                            boost::asio::ssl::context &ssl_ctx,
                                                            std::shared_ptr<SharedState> state);

} // namespace web_server
