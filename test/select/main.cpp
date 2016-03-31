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
    TEST_ASSERT_EQUAL(0, call_counter++);
}
void test_1()
{
    TEST_ASSERT_EQUAL(1, call_counter++);
}
void test_2_reset()
{
    greentea_send_kv("start", " ");
    char key[] = "state";
    char data[] = "10";
    greentea_parse_kv(key, data, sizeof(key), sizeof(data));
    
    uint8_t state = atoi(data);
    
    if (state == 0)
    {
        // change state to 1 and reset.
        greentea_send_kv("state", "1");
        greentea_send_kv("reset", " ");
    }
    else if (state == 1)
    {
        TEST_ASSERT_EQUAL(0, call_counter++);
        // jump to testcase 4
        greentea_send_kv("__test_start_from", "4");
    }
}
void test_3()
{
    TEST_ASSERT_EQUAL(1, call_counter++);
}
void test_4()
{
    TEST_ASSERT_EQUAL(1, call_counter++);
}

const Case cases[] =
{
    Case("Case 0", test_0),
    Case("Case 1", test_1),
    Case("SW reset", test_2_reset),
    Case("Case 3", test_3),
    Case("Case 4", test_4),
};

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    // Mock passed, failed counts (wait for API in utest)
    greentea_test_teardown_handler(4, 0, failure);
}

status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "reset_test");
    return greentea_test_setup_handler(number_of_cases);
}

const Specification specification(greentea_setup, cases, greentea_teardown, greentea_continue_handlers);

void app_start(int, char **) {
    Harness::run(specification);
}

