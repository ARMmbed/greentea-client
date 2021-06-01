/*
 * Copyright (c) 2021, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string>

#include <gtest/gtest.h>

#include "fake_console_io.h"
#include "greentea-client/test_env.h"

class KiViProtocolTest: public testing::Test {
public:
    Console fake_console;

protected:
    KiViProtocolTest() {}
    virtual ~KiViProtocolTest() {}
    virtual void TearDown() override
    {
        fake_console = {};
    }
};

TEST_F(KiViProtocolTest, SendStringValue)
{
    const std::string key = "hello";
    const std::string value = "99";
    const std::string output = "{{" + key + ";" + value + "}}\r\n";
    greentea_send_kv(key.c_str(), value.c_str());

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendIntValue)
{
    const std::string key = "hello";
    const int value = 99;
    const std::string output = "{{" + key + ";" + std::to_string(value) + "}}\r\n";

    greentea_send_kv(key.c_str(), value);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendIntPassFailCount)
{
    const std::string key = "hello";
    const int passes = 99;
    const int failures = 19;
    const std::string output = "{{" + key + ";" + std::to_string(passes) + ";" + std::to_string(failures) + "}}\r\n";

    greentea_send_kv(key.c_str(), passes, failures);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendStringValueAndResultCount)
{
    const std::string key = "hello";
    const std::string value = "hey";
    const int result = 99;
    const std::string output = "{{" + key + ";" + value + ";" + std::to_string(result) + "}}\r\n";

    greentea_send_kv(key.c_str(), value.c_str(), result);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}

TEST_F(KiViProtocolTest, SendStringValueAndPassFailCount)
{
    const std::string key = "hello";
    const std::string value = "hey";
    const int passes = 1;
    const int failures = 99;
    const std::string output = "{{" + key + ";" + value + ";" + std::to_string(passes) + ";" + std::to_string(failures) + "}}\r\n";

    greentea_send_kv(key.c_str(), value.c_str(), passes, failures);

    const std::string console = fake_console.get_stdout();
    ASSERT_EQ(console, output);
}
