# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.10)

project(greentea
    VERSION 0.1
    DESCRIPTION "Greentea client"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

add_library(client)
target_include_directories(client
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(client
    PRIVATE
        source/greentea_test_env.cpp
)

# Consumers using add_subdirectory should link to this target. This alias keeps
# the naming consistent between superprojects that include greentea-client in
# the source tree and projects that use the installed greentea-client library
# using find_package.
add_library(greentea::client ALIAS client)

# Exported targets

install(
    TARGETS client
    EXPORT greentea-client-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/greentea-client DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT greentea-client-targets
    FILE greentea-client-targets.cmake
    NAMESPACE greentea::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/greentea
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/greenteaConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/greentea
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/greenteaConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/greenteaConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/greenteaConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/greentea
)

export(
    EXPORT greentea-client-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/greentea-client-targets.cmake"
    NAMESPACE greentea::
)

# Default IO

SET(GREENTEA_CLIENT_STDIO ON CACHE BOOL "Use stdio for IO")

if(GREENTEA_CLIENT_STDIO)
    target_sources(client
        PRIVATE
            source/greentea_test_io.c
    )
endif()

# Example applications

set(BUILD_EXAMPLES ON CACHE BOOL "Enable building the examples")

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_EXAMPLES)
    if(GREENTEA_CLIENT_STDIO)
        add_subdirectory(examples/stdio)
    else()
        add_subdirectory(examples/custom_io)
        add_subdirectory(examples/pty)
    endif()
endif()
