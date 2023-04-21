/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "cache_bus.h"

// Blinking rate in milliseconds
#define BLINKING_RATE     100ms

DigitalOut led(PC_13);

DigitalIn pin1(PC_12);
DigitalIn pin2(PC_11);
DigitalIn pin3(PC_10);
DigitalIn pin4(PC_9);

AnalogIn pin5(PC_12);
AnalogIn pin6(PC_11);
AnalogIn pin7(PC_10);
AnalogIn pin8(PC_9);

void on_receive() {
    char c;
    led = !led;
}

int main()
{
    Cached::Digital digit = pin1;
    Cached::DBus<4> dbus {pin1, pin2, pin3, pin4};
    Cached::ABus<4> abus {pin5, pin6, pin7, pin8};

    digit.read();   //  updating cached value
    dbus.read_all(); // updating cached values for the hole bus

    while (true) {
        dbus.read<2, 3>();

        int d = dbus.get<3>();  // reads cached value
        int d2 = digit;         // reads cached value

        // float a = abus[4];           // no bound checking, undefined behaviour
        // float a2 = abus.get<4>();    // compile time error: array index out of bound

        ThisThread::sleep_for(100ms);
        led = !led;
    }
}
