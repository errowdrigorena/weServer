#pragma once

#include "web_server/store/memory_store.h"

#include <memory>

namespace web_server {

struct SharedState {
    MemoryStore store;
};

}  // namespace web_server

