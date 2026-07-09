include_guard(GLOBAL)

find_package(Boost REQUIRED CONFIG)
find_package(OpenSSL REQUIRED)

if(WEB_SERVER_BUILD_TESTS)
    find_package(GTest)
endif()
