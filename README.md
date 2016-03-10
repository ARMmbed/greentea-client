# greentea-client
```greentea-client``` is a DUT (device under test) client for [Greentea test tool](https://github.com/ARMmbed/greentea) used in [mbed](www.mbed.com) project.

* ```Greentea``` is test automation tool written in Python designed to automate test execution for [mbed](www.mbed.com) projects encapsulated in [yotta](http://yottadocs.mbed.com) modules.
* ```greentea-client``` is (written in C++03) [yotta module](http://yottadocs.mbed.com/reference/module.html). It resides on DUT and is ```Greentea``` counterpart. ```Greentea``` and ```greentea-client``` communicate with each other using KiVi protocol. It allows us to easily instrument test automation execution. Because ```greentea-client``` is already a yotta module you can easily add it as ```dependency```/```testDependency``` inside your [yotta module](http://yottadocs.mbed.com/reference/module.html).
  * This packages was introduces as ```devel_transport``` feature. See [here](https://github.com/ARMmbed/greentea/pull/78) for details. It changes how we write tests and may break your old test cases. If so you need to port your test cases to new model ```greentea-client``` promotes.
  * Test case porting examples can be found [here](https://github.com/ARMmbed/core-util/pull/95), [here](https://github.com/ARMmbed/ualloc/pull/24), [here](https://github.com/ARMmbed/minar/pull/36), [here](https://github.com/ARMmbed/mbed-drivers/pull/165), [here](https://github.com/ARMmbed/sal/pull/46), [here](https://github.com/ARMmbed/sockets/pull/64) and [here](https://github.com/ARMmbed/utest/pull/6).

## Compatibility
```greentea-client``` is compatible with:
* [Greentea](https://github.com/ARMmbed/greentea) v0.2.x onwards and
* [utest](https://github.com/ARMmbed/utest) v1.10.0 onwards.

### Greentea support

If you wish to use ```greentea-client``` please make sure you are using latest ```Greentea``` tools from [PyPI](https://pypi.python.org/pypi/mbed-greentea) (you will need Python 2.7):
```
$ pip install mbed-greentea --upgrade
```

*Note*: If you previously used ```mbed-drivers/test_env.h``` feature to write your test cases please downgrade to Greentea before v0.2.0:
```
$ pip install "mbed-greentea<0.2.0" --upgrade
```

*Note*: [Greentea v0.1.x](https://github.com/ARMmbed/greentea/tree/master_v0_1_x) is still developed on a devel branch ```master_v0_1_x```.

### utest support

*Note*: ```greentea-client``` should as test dependency automatically include compatible ```utest``` version. See ```greentea-client``` [module.json](https://github.com/ARMmbed/greentea-client/blob/master/module.json) ```testDependency``` section:
```
{
  "name": "greentea-client",
  "version": "0.1.8",

  ...

  "testDependencies": {
    "utest": "^1.10.0"
  }
}
```

### greentea-client support in your module
Currently ```greentea-client``` is in ```0.1.x``` version.
* *Note*: Please use ```~0.1.8``` dependency version in your ```module.json```.

Example of yotta module's ```module.json`` using ```greentea-client```:
```
{
  "name": "module-name",
  "dependencies": {

    ...

  },
  "testDependencies": {
    "greentea-client": "~0.1.8",
    "utest" : "^1.10.0",
    "unity" : "^2.1.0"
  }
}
```

## Vocabulary
* ```test suite``` - binary containing test cases we execute on hardware. Test suite has beginning and end (like your ```main()``` function would. Test suite may pass, fail or be in error state if for example test suite timeout or there was serial connection problem.
* ```test case``` - in most cases ```utest``` defined test case. It has beginning and end. During test case you will assert values, schedule callbacks, check for timeouts in your code. Your test cases may pass, fail or be in error state which means something went wrong and we were unable to determine what exactly (you may have to check logs).
* ```KiVi``` protocol - simple protocol introduced to Greentea test tools to send simple text messages between DUT and host.
  * KiVi event is defined as string encapsulated between double curly braces. Key and value are separated by semicolon ```;```. For example: ```{{timeout;120}}}``` is a simple key-value event where key "timeout" is associated with value "120". This can be parsed and recognized by both sides, DUT and host test.
  * Both key and value are ASCII strings.
  * This protocol is master-slave protocol. Master is host and slave is DUT (device under test).

## Where can I use it?
It is possible to write test cases which use ```greentea-client``` and at the same time support ```mbed``` features such as [MINAR scheduler](https://github.com/ARMmbed/minar). It is also possible to mix with ```greentea-client``` other test tools we use at ```mbed``` such as: [utest](https://github.com/ARMmbed/utest) and [unity](https://github.com/ARMmbed/unity).

You can also find references to ```greentea-client``` in many ```mbed``` packages. For example:
  * [mbed-drivers](https://github.com/ARMmbed/mbed-drivers) - check its [module.json](https://github.com/ARMmbed/mbed-drivers/blob/master/module.json)
  * [core-util](https://github.com/ARMmbed/core-util) - check its [module.json](https://github.com/ARMmbed/core-util/blob/master/module.json)

This package is designed to implement DUT side of protocol used to communicate between DUT and host computer. KiVi (Key-Value) is our simple protocol used to exchange events between DUT and host.

This package provides unified API designed to send to and receive from host KiVio events.

Extraction of implementation of:
* ```mbed-drivers``` [test_env.cpp](https://github.com/PrzemekWirkus/mbed-drivers/blob/master/source/test_env.cpp) and
* ```mbed-drivers``` [test_env.h](https://github.com/PrzemekWirkus/mbed-drivers/blob/master/mbed-drivers/test_env.h).

See [greentea](https://github.com/ARMmbed/mbed-drivers/pull/149) and latest changes regarding DUT-host transport layer.

# Test suite model

```c++
#include "greentea-client/test_env.h"

void app_start(int, char*[]) {
    bool result = true;
    GREENTEA_SETUP(15, "default_auto");

    // Test suite body
    // Here you can run your test cases and or assertions

    GREENTEA_TESTSUITE_RESULT(result);
}
```

Where:

```c++
void GREENTEA_SETUP(const int testsuite_timeout_sec, const char *host_test_script_name);
```

```c++
void GREENTEA_TESTSUITE_RESULT(const int testsuite_result);
```

# Test suite and test case examples
You can find many examples of test cases written

Note that tests written for [utest](https://github.com/ARMmbed/utest/tree/master/test) are instrumented with ```greentea-client```.

# yotta registry
You can find us here [greentea-client v0.1.8](https://yotta.mbed.com/#/module/greentea-client/0.1.8).
