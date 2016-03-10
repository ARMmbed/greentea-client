# greentea-client
```greentea-client``` is C++ client library for [Greentea test tool](https://github.com/ARMmbed/greentea) used in [mbed](www.mbed.com) project.
This package implements slave side of simple key-value protocol used to communicate between device under test (DUT) and host. ```Greentea``` in host side implements protocol's master behavior.

```
      DUT  <--- serial port connection --->   host
    (slave)         .                       (master)
                    .
[greentea-client]   .       [conn_process]               [htrun]
     =====          .      ================             =========
       |            .             |                         |
       |            .             |                         |
       |    {{ key ; value }}     |                         |
       |------------------------->| (key, value, timestamp) |
       |            .             |------------------------>|
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             | (key, value, timestamp) |
       |    {{ key ; value }}     |<------------------------|
       |<-------------------------|                         |
       |            .             |                         |
                    .
```

```greentea-client``` is [yotta module](http://yottadocs.mbed.com/reference/module.html). You can easily include it to your yotta project as ```dependency```/```testDependency```.
```greentea-client``` is released to [yotta registry](https://yotta.mbed.com/#/module/greentea-client/0.1.8).

## mbed-drivers dependencies
This package was introduced as future replacement for [mbed-drivers/test_env.h](https://github.com/ARMmbed/mbed-drivers/blob/master/mbed-drivers/test_env.h) test framework. ```mbed-drivers/test_env.h``` is no longer considered as the way of writing tests for mbed modules.

Examples of test case ported from old ```mbed-drivers/test_env``` to ```greentea-client``` model can be found [here](https://github.com/ARMmbed/core-util/pull/95), [here](https://github.com/ARMmbed/ualloc/pull/24), [here](https://github.com/ARMmbed/minar/pull/36), [here](https://github.com/ARMmbed/mbed-drivers/pull/165), [here](https://github.com/ARMmbed/sal/pull/46), [here](https://github.com/ARMmbed/sockets/pull/64) and [here](https://github.com/ARMmbed/utest/pull/6).

## Greentea test tools
```Greentea``` is test automation tool written in Python designed to automate test execution for [mbed](www.mbed.com) projects encapsulated in [yotta](http://yottadocs.mbed.com) modules. Its key features include: integration with yotta module, test automation for yotta module's tests and reporting.

## Compatibility
```greentea-client``` is compatible with:
* [Greentea](https://github.com/ARMmbed/greentea) v0.2.x onwards and
* [htrun](https://github.com/ARMmbed/htrun) v0.2.x onwards and
* [utest](https://github.com/ARMmbed/utest) v1.10.0 onwards and
* [unity](https://github.com/ARMmbed/utest) v2.0.1 onwards.

### Greentea support
If you wish to use ```greentea-client``` please make sure you are using latest ```Greentea``` tools from [PyPI](https://pypi.python.org/pypi/mbed-greentea) (you will need Python 2.7):
```
$ pip install mbed-greentea --upgrade
```

*Note*: If you previously used ```mbed-drivers/test_env.h``` features to write your test cases please downgrade to ```Greentea``` before ```v0.2.0``` to stay compatible:
```
$ pip install "mbed-greentea<0.2.0" --upgrade
```

*Note*: [Greentea v0.1.x](https://github.com/ARMmbed/greentea/tree/master_v0_1_x) is still developed on a ```master_v0_1_x``` branch. We will only apply critical patches to version 0.1.x, no feature development is planned.

### utest support
```utest``` is our preferred test harness which allows you to execute series of (asynchronous) C++ test cases.  ```greentea-client``` includes as test dependency to compatible ```utest``` version. See ```greentea-client``` [module.json](https://github.com/ARMmbed/greentea-client/blob/master/module.json) ```testDependency``` section:
```json
{
  "testDependencies": {
    "utest": "^1.10.0"
  }
}
```

### greentea-client support in your module
Currently ```greentea-client``` is in ```0.1.x``` version. Please use ```~0.1.8``` dependency version in your ```module.json``` file.

Example of ```module.json``` file with ```greentea-client``` as test dependency:
```json
{
  "testDependencies": {
    "greentea-client": "~0.1.8",
    "utest" : "^1.10.0",
    "unity" : "^2.1.0"
  }
}
```

## Terms
### Test suite
Test suite is a binary containing test cases we execute on hardware. Test suite has the beginning and the end (like your ```main()``` function would. Test suite may pass, fail or be in error state if for example test suite timeout or there was serial port connection problem.
### Test case
Preferably you will use ```utest``` to define test cases . test case has the beginning and the end. During test case execution you will use ``ùnity``` assert macros, schedule MINAR callbacks, check for timeouts in your code. Your test cases may pass, fail or be in error state which means something went wrong and we were unable to determine what exactly (you may have to check logs).
### key-value protocol
Key-value protocol (also called ```KiVi``` protocol) is a simple protocol introduced to ```Greentea``` test tools. It is used to send simple text messages (events) between DUT and host. Each message consists of _key_ and corresponding _value_ pair.
```KiVi``` message is defined as a string encapsulated between double curly braces. Key and value are separated by semicolon ```;```.
For example: ```{{timeout;120}}}``` string is a simple key-value message where key "_timeout_" is associated with value "_120_". Both ```greentea-client``` and ```Greentea``` understand this format and can detect key-value messages in stream of data. Both _key_ and _value_ are ASCII strings.
This protocol is a master-slave protocol. Host has a role of _master_ and DUT is _slave_.

```greentea-client``` implements key-value protocol tokenizer and parser.

## Where can I use it?
It is possible to write test cases which use ```greentea-client``` and at the same time support ```mbed``` features such as [MINAR scheduler](https://github.com/ARMmbed/minar). It is also possible to mix with ```greentea-client``` other test tools we use at ```mbed``` such as: [utest](https://github.com/ARMmbed/utest) and [unity](https://github.com/ARMmbed/unity).

You can also find references to ```greentea-client``` in many ```mbed``` packages. For example:
  * [mbed-drivers](https://github.com/ARMmbed/mbed-drivers) - check its [module.json](https://github.com/ARMmbed/mbed-drivers/blob/master/module.json)
  * [core-util](https://github.com/ARMmbed/core-util) - check its [module.json](https://github.com/ARMmbed/core-util/blob/master/module.json)

# Test suite model
## utest support template
```c++
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

void test_case_1_func() {
    // Test case #1 body
    // Here you can run your test cases and or assertions
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
}

void test_case_2_func() {
    // Test case #2 body
    // Here you can run your test cases and or assertions
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
}

const Case cases[] = {
    Case("Test case #1 name", test_case_1_func),
    Case("Test case #1 name", test_case_2_func)
};

status_t greentea_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

void app_start(int, char*[]) {
    Harness::run(specification);
}
```
## No utest support template
```c++
#include "greentea-client/test_env.h"
#include "unity/unity.h"

void app_start(int, char*[]) {
    bool result = true;
    GREENTEA_SETUP(15, "default_auto");

    // Test suite body
    // Here you can run your test cases and or assertions
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);

    GREENTEA_TESTSUITE_RESULT(result);
}
```
