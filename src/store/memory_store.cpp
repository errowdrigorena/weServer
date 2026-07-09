#include "web_server/store/memory_store.h"

namespace web_server {

std::vector<Item> MemoryStore::list() const
{
    std::vector<Item> result;
    result.reserve(items_.size());
    for (auto const& [id, item] : items_) {
        result.push_back(item);
    }
    return result;
}

std::optional<Item> MemoryStore::get(std::int64_t id) const
{
    auto it = items_.find(id);
    if (it == items_.end()) {
        return std::nullopt;
    }
    return it->second;
}

Item MemoryStore::create(Item item)
{
    item.id = next_id_++;
    items_.emplace(item.id, item);
    return item;
}

std::optional<Item> MemoryStore::update(std::int64_t id, Item item)
{
    auto it = items_.find(id);
    if (it == items_.end()) {
        return std::nullopt;
    }
    item.id = id;
    it->second = std::move(item);
    return it->second;
}

bool MemoryStore::erase(std::int64_t id)
{
    return items_.erase(id) > 0;
}

}  // namespace web_server
