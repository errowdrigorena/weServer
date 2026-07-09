add_library(weServer_protocol STATIC
    error.cpp
    request_context.cpp
    server_certificate.cpp
)

add_library(weServer::protocol ALIAS weServer_protocol)

weserver_configure_library(weServer_protocol)

target_link_libraries(weServer_protocol
    PUBLIC
        Boost::headers
        OpenSSL::SSL
        OpenSSL::Crypto
)
