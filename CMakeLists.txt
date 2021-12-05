cmake_minimum_required( VERSION 3.10 )
project( tableprinter )

add_library( tableprinter INTERFACE )
add_library( tableprinter::tableprinter ALIAS tableprinter )
target_compile_features( tableprinter INTERFACE cxx_std_17 )

target_include_directories(
    tableprinter INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:src>
)

include( GNUInstallDirs )

install(
    TARGETS tableprinter
    EXPORT tableprinter-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/tableprinter
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include( CMakePackageConfigHelpers )

install(
    EXPORT tableprinter-targets
    FILE tableprinter-targets.cmake
    NAMESPACE tableprinter::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tableprinter
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/tableprinter-config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/tableprinter-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tableprinter
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/tableprinter-config-version.cmake
    VERSION 3.1
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
    ${CMAKE_BINARY_DIR}/cmake/tableprinter-config.cmake
    ${CMAKE_BINARY_DIR}/cmake/tableprinter-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tableprinter
)

export( EXPORT tableprinter-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/tableprinter-targets.cmake
    NAMESPACE tableprinter::
)

add_subdirectory( example )
add_subdirectory( benchmark )
enable_testing()
add_subdirectory( test )