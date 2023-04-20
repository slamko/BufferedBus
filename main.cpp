/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "buf_bus.h"

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
    Buffered::DBus<4> dbus {pin1, pin2, pin3, pin4};
    Buffered::ABus<4> abus {pin5, pin6, pin7, pin8};
    dbus.read();

    while (true) {
        int d = dbus.get<3>();
        float a = abus[4];

        ThisThread::sleep_for(100ms);
        led = !led;
    }
}
