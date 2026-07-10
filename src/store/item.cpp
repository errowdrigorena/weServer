#include "web_server/store/item.h"

#include <boost/json/parse.hpp>
#include <boost/system/error_code.hpp>

namespace web_server {
namespace {

enum class ItemErrc {
    invalid_json = 1,
    missing_field,
};

class ItemErrorCategory final : public boost::system::error_category {
  public:
    [[nodiscard]] char const *name() const noexcept override {
        return "item";
    }

    [[nodiscard]] std::string message(int ev) const override {
        switch (static_cast<ItemErrc>(ev)) {
        case ItemErrc::invalid_json:
            return "invalid JSON body";
        case ItemErrc::missing_field:
            return "missing required field";
        default:
            return "unknown item error";
        }
    }
};

[[nodiscard]] ItemErrorCategory const &item_error_category() {
    static ItemErrorCategory const instance;
    return instance;
}

[[nodiscard]] boost::system::error_code make_item_error(ItemErrc err) {
    return {static_cast<int>(err), item_error_category()};
}

} // namespace

boost::json::object Item::to_json() const {
    return {
        {"id", id},
        {"name", name},
        {"description", description},
    };
}

boost::system::result<Item> item_from_json(std::string_view body) {
    boost::system::error_code ec;
    auto value = boost::json::parse(body, ec);
    if (ec) {
        return make_item_error(ItemErrc::invalid_json);
    }

    if (!value.is_object()) {
        return make_item_error(ItemErrc::invalid_json);
    }

    auto const &obj = value.as_object();
    auto const name_it = obj.find("name");
    if (name_it == obj.end() || !name_it->value().is_string()) {
        return make_item_error(ItemErrc::missing_field);
    }

    Item item;
    item.name = name_it->value().as_string().c_str();

    if (auto const desc_it = obj.find("description"); desc_it != obj.end()) {
        if (!desc_it->value().is_string()) {
            return make_item_error(ItemErrc::invalid_json);
        }
        item.description = desc_it->value().as_string().c_str();
    }

    return item;
}

} // namespace web_server
