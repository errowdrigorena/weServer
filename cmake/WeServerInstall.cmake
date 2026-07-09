include_guard(GLOBAL)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(weserver_cmake_targets
    weServer_warnings
    weServer_options
    weServer_include
    weServer_store
    weServer_protocol
    weServer_crud
    weServer_net
)

install(
    TARGETS
        ${weserver_cmake_targets}
        weserver
    EXPORT weServerTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/web_server/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/web_server
)

install(
    EXPORT weServerTargets
    FILE weServerTargets.cmake
    NAMESPACE weServer::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/weServer
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/WeServerConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/weServerConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/weServer
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/weServerConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/weServerConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/weServerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/weServer
)
