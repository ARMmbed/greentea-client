# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(fake-console-io ./fake_console_io.cpp)
target_compile_features(fake-console-io PUBLIC cxx_std_14)
target_include_directories(fake-console-io PUBLIC .)
target_link_libraries(fake-console-io PRIVATE greentea::client_userio)
