/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* @file main.cpp
 *
 * This test suite tests scenarios to reset target in-between the test.
 *
 *   The following tests are implemented in this file:
 *     Test 1            Reset target inbetween the tests.
 */

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include <stdlib.h>


using namespace utest::v1;

int call_counter(0);

void test_0()
{
    printf("Execute Test 0\n");
}
void test_1()
{
    printf("Execute Test 1\n");
}
void test_2()
{
    printf("Execute Test 2\n");
}
void test_3()
{
    printf("Execute Test 3\n");
}
void test_4()
{
    printf("Execute Test 4\n");
}

const Case cases[] =
{
    Case("Case 0", test_0),
    Case("Case 1", test_1),
    Case("Case 2", test_2),
    Case("Case 3", test_3),
    Case("Case 4", test_4),
};

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    greentea_test_teardown_handler(passed, failed, failure);
}

status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    greentea_send_kv("__shuffle", " ");
    //greentea_send_kv("__shuffle_seed", "0.537925");
    return greentea_test_setup_handler(number_of_cases);
}

const Specification specification(greentea_setup, cases, greentea_teardown, greentea_continue_handlers);

void app_start(int, char **) {
    Harness::run(specification);
}

