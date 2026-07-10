#pragma once

#include <boost/asio/ssl/context.hpp>

namespace web_server {

void load_server_certificate(boost::asio::ssl::context &ctx);

} // namespace web_server
