add_library(weServer_crud STATIC
    items.cpp
)

add_library(weServer::crud ALIAS weServer_crud)

weserver_configure_library(weServer_crud)

target_link_libraries(weServer_crud
    PUBLIC
        weServer::protocol
        weServer::store
)
