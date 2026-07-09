add_subdirectory(store)
add_subdirectory(protocol)
add_subdirectory(crud)
add_subdirectory(net)

add_executable(weserver main.cpp)
weserver_configure_executable(weserver)
target_link_libraries(weserver PRIVATE weServer::net)
