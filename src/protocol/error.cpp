#include "web_server/error.h"

#include <boost/asio/ssl/error.hpp>

#include <iostream>

namespace web_server {

void log_error(boost::system::error_code ec, std::string_view what, std::string_view detail) {
    if (ec == boost::asio::ssl::error::stream_truncated) {
        return;
    }

    std::cerr << what << ": " << ec.message();
    if (!detail.empty()) {
        std::cerr << " (" << detail << ")";
    }
    std::cerr << '\n';
}

} // namespace web_server
