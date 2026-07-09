#pragma once

#include "web_server/store/item.h"

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

namespace web_server {

// In-memory persistence until a real database is wired in.
class MemoryStore {
public:
    [[nodiscard]] std::vector<Item> list() const;
    [[nodiscard]] std::optional<Item> get(std::int64_t id) const;
    [[nodiscard]] Item create(Item item);
    [[nodiscard]] std::optional<Item> update(std::int64_t id, Item item);
    [[nodiscard]] bool erase(std::int64_t id);

private:
    std::int64_t next_id_{1};
    std::unordered_map<std::int64_t, Item> items_;
};

}  // namespace web_server
