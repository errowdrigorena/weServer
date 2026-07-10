#pragma once

#include <boost/json/object.hpp>
#include <boost/system/result.hpp>

#include <cstdint>
#include <string>
#include <string_view>

namespace web_server {

struct Item {
    std::int64_t id{0};
    std::string name;
    std::string description;

    [[nodiscard]] boost::json::object to_json() const;
};

[[nodiscard]] boost::system::result<Item> item_from_json(std::string_view body);

} // namespace web_server
