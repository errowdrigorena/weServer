#include "web_server/store/memory_store.h"

#include <gtest/gtest.h>

namespace web_server {
namespace {

TEST(MemoryStoreTest, CreateAndList) {
    MemoryStore store;
    Item item;
    item.name = "alpha";
    item.description = "first";

    auto const created = store.create(item);
    EXPECT_EQ(created.id, 1);
    EXPECT_EQ(created.name, "alpha");

    auto const items = store.list();
    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items.front().id, 1);
}

TEST(MemoryStoreTest, GetUpdateDelete) {
    MemoryStore store;
    Item item;
    item.name = "beta";
    auto const created = store.create(item);

    auto const fetched = store.get(created.id);
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->name, "beta");

    Item updated;
    updated.name = "gamma";
    auto const result = store.update(created.id, updated);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "gamma");

    EXPECT_TRUE(store.erase(created.id));
    EXPECT_FALSE(store.get(created.id).has_value());
}

} // namespace
} // namespace web_server
