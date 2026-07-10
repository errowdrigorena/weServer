#pragma once

#include <chrono>
#include <cstddef>
#include <string_view>

namespace web_server {

inline constexpr std::string_view crud_items_path{"/crud/items"};
inline constexpr char const default_listen_address[]{"127.0.0.1"};
inline constexpr unsigned short default_listen_port{8080};
inline constexpr std::size_t max_request_body_size{10'000};
inline constexpr auto io_timeout{std::chrono::seconds{30}};

} // namespace web_server
