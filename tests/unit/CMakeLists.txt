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
target_compile_features(greentea-tests PUBLIC cxx_std_14)
target_link_libraries(greentea-tests PUBLIC greentea::client_userio fake-console-io gtest_main)
gtest_discover_tests(greentea-tests DISCOVERY_MODE PRE_TEST)

# Coverage

option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    target_compile_options(client_userio
        PUBLIC
            --coverage -O0 -g
    )
    target_link_options(client_userio
        PUBLIC
            --coverage
    )
endif()
