include_guard(GLOBAL)

add_library(weServer_warnings INTERFACE)

if(MSVC)
    target_compile_options(weServer_warnings INTERFACE /W4 /permissive-)
else()
    target_compile_options(
        weServer_warnings
        INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
            -Wnon-virtual-dtor
    )
endif()

add_library(weServer_options INTERFACE)
target_compile_features(weServer_options INTERFACE cxx_std_20)
target_compile_definitions(
    weServer_options
    INTERFACE
        BOOST_BEAST_USE_STD_STRING_VIEW
)
target_link_libraries(weServer_options INTERFACE weServer_warnings)

add_library(weServer_include INTERFACE)
target_include_directories(
    weServer_include
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(weServer::warnings ALIAS weServer_warnings)
add_library(weServer::options ALIAS weServer_options)
add_library(weServer::include ALIAS weServer_include)

function(weserver_configure_library target)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "weserver_configure_library: unknown target '${target}'")
    endif()

    target_link_libraries(
        ${target}
        PUBLIC
            weServer::include
            weServer::options
    )
endfunction()

function(weserver_configure_executable target)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "weserver_configure_executable: unknown target '${target}'")
    endif()

    target_link_libraries(
        ${target}
        PRIVATE
            weServer::options
    )
endfunction()
