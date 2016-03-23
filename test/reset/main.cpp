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

#include "mbed-drivers/mbed.h"

#include <algorithm>
#include <cstdlib>

using namespace utest::v1;

/** Custom echo test
 * 
 *  This function sends key,value pairs like {{echo;n}}. Where n is a number.
 *  Host replies back with {{echo;m*n}}. Where m is a multiplier both target
 *  and host know based on the target state.
 *
 *        state     multiplier
 *          0           1
 *          1           2
 *
 *  Response is asserted to verify the state based execution of both host
 *  and target before and after reset.
 */
void do_echo_test(int response_multiplier)
{
    char key[] = "echo";
    char data[] = "10";

    for (int i =0; i < 10; i++)
    {
        greentea_send_kv("echo", i);

        TEST_ASSERT_NOT_EQUAL_MESSAGE(0, greentea_parse_kv(key, data, sizeof(key), sizeof(data)), "MBED: Failed to recv/parse key \"state\" value from host test!");
        TEST_ASSERT_EQUAL_STRING("echo", key);

        uint8_t count = atoi(data);

        TEST_ASSERT_EQUAL_MESSAGE(i * response_multiplier, count, "MBED: Failed to verify count received in echo!");
    }
}


/** Reset test
 * 
 *  This function executes the reset test. It kicks off the host test by sedning
 *  {{start;}}
 *  Then receives state from host {{state;x}}. Where it should be x=0 on first start and 1
 *  after reset.
 *  In both before and after iterations it runs an echo test. It verifies the echoes based
 *  on a multiplier known to both target and host test code based on the target state.
 *
 *        state     multiplier
 *          0           1
 *          1           2
 *
 */
void reset_test_cb()
{
    // Start the test
    greentea_send_kv("start", " ");

    // A startup read state from the host
    char key[] = "state";
    char data[] = "10";
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0, greentea_parse_kv(key, data, sizeof(key), sizeof(data)), "MBED: Failed to recv/parse key \"state\" value from host test!");

    uint8_t state = atoi(data);
    
    TEST_ASSERT_TRUE(state <= 1);

    // Start echo test with a response multiplier that is shared secret between target and host to demonstrated
    // state correspondance. 
    // state   multiplier
    //   0        1
    //   1        2

    if (state == 0)
    {
        do_echo_test(1);

        // change state to 1 and reset.
        greentea_send_kv("state", "1");
        greentea_send_kv("reset", " ");
    }
    else if (state == 1)
    {
        do_echo_test(2);
        Harness::validate_callback();
    }

}

control_t reset_test(const size_t)
{
    minar::Scheduler::postCallback(reset_test_cb).delay(minar::milliseconds(100));
    return CaseAwait;
}



const Case cases[] =
{
    Case("Reset Test", reset_test),
};

void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    greentea_test_teardown_handler(passed, failed, failure);
}

status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "reset_test");
    return greentea_test_setup_handler(number_of_cases);
}

const Specification specification(greentea_setup, cases, greentea_teardown);

void app_start(int, char **) {
    Harness::run(specification);
}

