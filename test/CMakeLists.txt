add_executable(weServer_store_tests test_memory_store.cpp)

weserver_configure_executable(weServer_store_tests)

target_link_libraries(weServer_store_tests
    PRIVATE
        weServer::store
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(weServer_store_tests)
