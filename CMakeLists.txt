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

# Metrics

SET(GREENTEA_CLIENT_DUMMY_METRICS ON CACHE BOOL "Use dummy no-op implementation of metrics")

if(GREENTEA_CLIENT_DUMMY_METRICS)
    target_sources(greentea-client
        PRIVATE
            source/greentea_metrics.cpp
    )
endif()
