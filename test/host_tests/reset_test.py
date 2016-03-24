# Copyright 2015 ARM Limited, All rights reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from mbed_host_tests import BaseHostTest, event_callback


class ResetTest(BaseHostTest):
    """
    In this test target requests a reset by sending {{reset;}} key.
    Host test manages target state before and after reset. On start
    up host test sends initial state to the target using key:
    {{state;<state>}}

    In state==0 target starts an echo test. Target validates the echoes
    to be same as the sent. Example: Sent {{echo;1}} should results in
    receiving {{echo;1}}. In the end it changes state to 1 and
    sends state to host test and requests reset.

    Host test resets the target and target startup happens as before.
    Host test sends stored state to the target. 

    In state==1 target starts an echo test and expects echoes with
    n x 2. Example: if echo is {{echo;1}} then response should be
    {{echo;2}}. Target validates the echoes and finishes the test
    after 10 echoes.
    """

    def __init__(self):
        """
        Initialise test parameters.

        :return:
        """
        super(BaseHostTest, self).__init__()
        self.state = 0 # Default/start state until updated by the target

    @event_callback("start")
    def _callback_start(self, key, value, timestamp):
        """
        Key "start" handler to initiate the test.

        :param key:
        :param value:
        :param timestamp:
        :return:
        """
        # Tell stored state to the target
        self.send_kv("state", self.state)

    @event_callback("echo")
    def _callback_echo(self, key, value, timestamp):
        """
        Key "echo" handler. Echoes back the received number after multiplying with
        a state corresponding multiplier.

        state   multiplier
        0       1
        1       2

        :param key:
        :param value:
        :param timestamp:
        :return:
        """
        assert self.state in [0, 1], "Unknown state!"
        if self.state == 0:
            multiplier = 1
        elif self.state == 1:
            multiplier = 2
        self.send_kv("echo", multiplier * int(value))

    @event_callback("state")
    def _callback_state(self, key, value, timestamp):
        """
        Key "state" handler. Updateds the target state.

        :param key:
        :param value:
        :param timestamp:
        :return:
        """
        self.state = int(value)

    @event_callback("reset")
    def _callback_reset(self, key, value, timestamp):
        """
        Key "reset" handler. Resets the target and on come back reinitializes to last known state.

        :param key:
        :param value:
        :param timestamp:
        :return:
        """
        self.reset_dut(value)

    def teardown(self):
        """
        Test tear down handler.
        """
        pass

