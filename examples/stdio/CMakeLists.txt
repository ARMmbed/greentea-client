# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_executable(greentea-client-example main.cpp)

target_link_libraries(greentea-client-example
        greentea::client
)
