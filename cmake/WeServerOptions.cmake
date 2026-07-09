include_guard(GLOBAL)

option(WEB_SERVER_BUILD_TESTS "Build unit tests" ON)
option(WESERVER_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(WESERVER_DEVELOPER_MODE "Export compile_commands and enable strict defaults" ON)

if(WESERVER_DEVELOPER_MODE)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WESERVER_WARNINGS_AS_ERRORS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Werror)
    elseif(MSVC)
        add_compile_options(/WX)
    endif()
endif()
