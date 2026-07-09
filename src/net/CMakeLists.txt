add_library(weServer_net STATIC
    http_session.cpp
    server.cpp
)

add_library(weServer::net ALIAS weServer_net)

weserver_configure_library(weServer_net)

target_link_libraries(weServer_net
    PUBLIC
        weServer::crud
)
