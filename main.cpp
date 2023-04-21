/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "examples.h"

// Blinking rate in milliseconds
#define BLINKING_RATE     100ms

DigitalOut led(PC_13);

int main()
{
   
    while (true) {
     
        ThisThread::sleep_for(100ms);
        led = !led;
    }
}
