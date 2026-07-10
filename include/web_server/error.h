#pragma once

#include <boost/system/error_code.hpp>

#include <string_view>

namespace web_server {

void log_error(boost::system::error_code ec, std::string_view what, std::string_view detail = {});

} // namespace web_server
