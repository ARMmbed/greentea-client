# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.10)

project(greentea-client
    VERSION 0.1
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

# Default IO

SET(GREENTEA_CLIENT_STDIO ON CACHE BOOL "Use stdio for IO")

if(GREENTEA_CLIENT_STDIO)
    target_sources(greentea-client
        PRIVATE
            source/greentea_test_io.c
    )
endif()

# Example application

set(BUILD_EXAMPLES ON CACHE BOOL "Enable building the examples")

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
