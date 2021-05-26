# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.10)

include(cmake/GitVersion.cmake)

message("version ${GIT_VERSION}")
message("major ${GIT_VERSION_MAJOR}")
message("minor ${GIT_VERSION_MINOR}")
message("patch ${GIT_VERSION_PATCH}")

project(greentea-client
    VERSION ${GIT_VERSION_MAJOR}.${GIT_VERSION_MINOR}.${GIT_VERSION_PATCH}
    DESCRIPTION "Greentea client"
    LANGUAGES C CXX
)

add_library(greentea-client)

target_include_directories(greentea-client
    PUBLIC
        include
)

target_sources(greentea-client
    PRIVATE
        source/greentea_test_env.cpp
)

target_compile_definitions(greentea-client
    PUBLIC
        GIT_VERSION=${GIT_VERSION}
)

# Default IO

SET(GREENTEA_CLIENT_STDIO ON CACHE BOOL "Use stdio for IO")

if(GREENTEA_CLIENT_STDIO)
    target_sources(greentea-client
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
    endif()
endif()
