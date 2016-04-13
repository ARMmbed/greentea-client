/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "greentea-client/state_in.h"

#include "mbed-drivers/PortIn.h"
#include "mbed-drivers/DigitalOut.h"
#include "mbed-drivers/InterruptIn.h"
#include "minar/minar.h"

#include <stdio.h>
/**
 * @file state_out.cpp
 * @brief report application states via PortOut
 *
 * This implementation of state_out works only for k64f, and requires the following pins:
 * PTC0, PTC1, PTC2, PTC3, PTC4, PTC5, PTC7, PTC8, PTC9, PTD0
 * The PortOut API is used to keep the time used by state_out to a minimum
 */

#ifdef TARGET_LIKE_FRDM_K64F
/** The state bus
 *
 * This bus provides fast communication with the DUT
 */
mbed::PortIn      StateIn(PortC,0x1BF);
/** State Stable signal
 *
 * The DUT uses this signal to indicate that the state is stable
 */
mbed::InterruptIn StateStrobe(PTC9);
/** State Acknowledge
 *
 * The test jig uses this signal to acknowledge the state strobe and to indicate when the
 * state processing has finished.
 */
mbed::DigitalOut  StateAck(PTD0);

/** The function to call when a new state is reported */
mbed::util::FunctionPointer1<void, uint8_t> onRise;

/** Strobe Callback
 *
 * This function is called when the state strobe goes high. It processes the raw PortIn and converts it to an 8-bit
 * state. Then, it calls the onRise function before acknowledging the state.
 */
static void strobecb() {
    uint32_t state = StateIn.read();
    state = ((state >> 1) & ~0x3F) | (state & 0x3F);
    if (onRise)
    {
        onRise(state);
    }
    StateAck = 1;
    while (StateStrobe.read())
    {}
    StateAck = 0;
}

/** Initialize the state_in API
 *
 * Sets the onRise callback, sets the callback for the strobe IRQ, and indicates it is ready to receive state inputs by
 * deasserting the Acknowledge signal.
 *
 * @param[in] cb the function to call when a state change has occurred. This should be a short function callable from
 *               IRQ context.
 */
void set_state_in_cb(state_in_cb cb) {
    onRise = cb;
    StateStrobe.rise(strobecb);
    StateAck = 0;
}
#else
#warning  The only supported target for state_out is frdm-k64f
void state_out(uint8_t state)
{}
#endif
