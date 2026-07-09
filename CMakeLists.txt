cmake_minimum_required(VERSION 3.23)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(weServer
    VERSION 0.1.0
    DESCRIPTION "HTTPS CRUD server based on Boost.Beast coroutines"
    LANGUAGES CXX
)

include(WeServerOptions)
include(WeServerDependencies)
include(WeServerTargets)

add_subdirectory(src)

if(WEB_SERVER_BUILD_TESTS AND GTest_FOUND)
    enable_testing()
    add_subdirectory(test)
elseif(WEB_SERVER_BUILD_TESTS)
    message(STATUS "GTest not found: unit tests disabled")
endif()

include(WeServerInstall)
