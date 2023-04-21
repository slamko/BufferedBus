// some examples of using cached bus

#include "mbed.h"
#include "cache_bus.h"

DigitalIn pin1(PC_12);
DigitalIn pin2(PC_11);
DigitalIn pin3(PC_10);
DigitalIn pin4(PC_9);

AnalogIn pin5(PC_12);
AnalogIn pin6(PC_11);
AnalogIn pin7(PC_10);
AnalogIn pin8(PC_9);

int example_main()
{
    Cached::Digital digit = pin1;
    Cached::DBus<4> dbus {pin1, pin2, pin3, pin4}; 
    // equivalent to Cached::Bus<Cached::Digital, 4> {pin1 ... }

    Cached::ABus<4> abus {pin5, pin6, pin7, pin8};
    // equivalent to Cached::Bus<Cached::Analog, 4> {pin5 ... }

    digit.read();   //  updating cached value
    dbus.read_all(); // updating cached values for the hole bus

    while (true) {
        dbus.read<0, 2, 3>();   // compile-time bound checking
        //dbus.read({0, 1, 5});   // no bound checking (UB)

        int d = dbus.get<3>();  // reads cached value (the value of pin2 is never updated)
        int d2 = digit;         // reads cached value

        // float a = abus[4];           // no bound checking, undefined behaviour
        // float a2 = abus.get<4>();    // compile time error: array index out of bound
    }
}

// kinda more useful example

void calculate(const Cached::DBus<4> &bus) {
    // ...
}

int example_main2()
{
    Cached::DBus<4> dbus {pin1, pin2, pin3, pin4}; 
    
    while (true) {
        dbus.read_all();   
        calculate(dbus);
    }
}

