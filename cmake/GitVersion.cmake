# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

execute_process(COMMAND git describe --tags --abbrev=12 --dirty --always
                OUTPUT_VARIABLE GIT_VERSION
                ERROR_QUIET)

if (GIT_VERSION MATCHES "^v([0-9]+)\.([0-9]+)\.([0-9]+)")
    set(GIT_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(GIT_VERSION_MINOR ${CMAKE_MATCH_2})
    set(GIT_VERSION_PATCH ${CMAKE_MATCH_3})
else()
    set(GIT_VERSION_MAJOR 0)
    set(GIT_VERSION_MINOR 0)
    set(GIT_VERSION_PATCH 0)
endif()
