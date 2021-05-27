# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(googletest)

add_executable(greentea-tests test_kv_protocol.cpp)
target_link_libraries(greentea-tests PRIVATE greentea::client gtest_main)
gtest_discover_tests(greentea-tests DISCOVERY_MODE PRE_TEST)

# Coverage

option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    target_compile_options(greentea-tests
        PUBLIC
            --coverage -O0 -g
    )
    target_link_options(greentea-tests
        PUBLIC
            --coverage
    )
endif()
