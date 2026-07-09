add_library(weServer_store STATIC
    item.cpp
    memory_store.cpp
)

add_library(weServer::store ALIAS weServer_store)

weserver_configure_library(weServer_store)

target_link_libraries(weServer_store
    PUBLIC
        Boost::json
)
