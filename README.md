# Table of Contents

* [greentea-client](#greentea-client)
  * [Compatibility](#compatibility)
  * [Terms](#terms)
    * [Test suite](#test-suite)
    * [Test case](#test-case)
    * [key-value protocol](#key-value-protocol)
* [greentea-client example](#greentea-client-example)

# greentea-client

[greentea-client](https://github.com/ARMmbed/greentea-client.git) is a C++ client library which can used with the [the Greentea test tool](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-greentea). This package implements the slave side of the simple key-value protocol used for communication between the device under test (DUT) and the host. ```Greentea``` on the host side implements the protocol's master behaviour.


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

## Compatibility

```greentea-client``` is compatible with:
* [Greentea](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-greentea)
* [htrun](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-host-tests)

## Terms

### Test suite
A test suite is a binary containing test cases we execute on hardware. The test suite has a beginning and an end (like your ```main()``` function would. The test suite may pass, fail or be in an error state (for example if the test suite times out or there was a serial port connection problem).

### Test case

A test case is a set of actions executed to verify a particular functionality. It contains test steps, test data, precondition, postcondition for specific test scenario to verify that functionality requirement. The test case has the beginning and the end. During test case execution you will use assert macros, schedule callbacks, check for timeouts in your code. Your test cases may pass, fail or be in an error state which means something went wrong and we were unable to determine exactly what that was (you may have to check the logs).

### key-value protocol

The key-value protocol (also called ```KiVi```) is a simple protocol introduced to the ```Greentea``` test tools. It is used to send simple text messages (events) between the DUT and the host. Each message consists of a _key_ and corresponding _value_ pair. 
A ```KiVi``` message is defined as a string encapsulated between double curly braces. The key and value are separated by a semicolon (```;```).
For example, the ```{{timeout;120}}}``` string is a simple key-value message where the key "_timeout_" is associated with the value "_120_". Both ```greentea-client``` and ```Greentea``` understand this format and can detect key-value messages in a data stream. Both _key_ and _value_ are ASCII strings.
This protocol is a master-slave protocol. The host has the role of _master_ and the DUT is the _slave_.

```greentea-client``` implements the key-value protocol tokenizer and parser.

### greentea-client example

You can find the greentea-client example application [here](./example/main.cpp). To build it,

    cmake -S . -B cmake_build -GNinja
    cmake --build cmake_build

This generates an executable `./cmake_build/example/greentea-client-example`.
