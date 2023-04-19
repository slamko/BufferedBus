/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "buffered_in.cpp"

// Blinking rate in milliseconds
#define BLINKING_RATE     100ms

DigitalOut led(PC_13);
DigitalIn pin(PC_12);
DigitalIn pin1(PC_12);

void on_receive() {
    char c;
    led = !led;
}

int main()
{
    Buffered::Digital p = pin;
    Buffered::Bus bus {pin1, pin};

    while (true) {

        ThisThread::sleep_for(100ms);
        led = !led;
    }
}
