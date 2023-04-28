// some examples of using cached bus

#include "mbed.h"
#include "cache_bus.h"
#include <tuple>

DigitalIn pin1(PC_12);
DigitalIn pin2(PC_11);
DigitalIn pin3(PC_10);
DigitalIn pin4(PC_9);

AnalogIn pin5(PC_12);
AnalogIn pin6(PC_11);
AnalogIn pin7(PC_10);
AnalogIn pin8(PC_9);

using namespace Cached;

// variadic bus (aka mixed types)
VBus<Digital, Digital, Analog, Digital, Analog, Digital> vbus {
    true,
    DigitalIn(PC_7), 
    DigitalIn(PC_8), 
    AnalogIn(PC_9), 
    DigitalIn(PC_10), 
    AnalogIn(PC_11), 
    DigitalIn(PC_12)
}; 

// handy helper
auto bus = make_vbus(
    true,
    DigitalIn(PC_7), 
    AnalogIn(PC_4),
    DigitalIn(PC_8), 
    DigitalIn(PC_10),
    DigitalIn(PC_11) 
);

int example_main()
{
    Digital digit {pin1};

    // equivalent to Cached::Bus<Cached::Digital, 4> {pin1 ... }
    Cached::DBus<4> dbus {DigitalIn{PC_4}, pin2, pin3, pin4}; 

    // equivalent to Cached::Bus<Cached::Analog, 4> {pin5 ... }
    Cached::ABus<4> abus {pin5, pin6, pin7, pin8};

    digit.read();   //  updating cached value
    auto [l1, l2, l3, l4, l5, l6] = vbus.read_all(); // updating cached values for the hole bus

    while (true) {
        //(updating cache only for pin1, pin3 and pin4)
        dbus.read<0, 2, 3>();   // compile-time bound checking 
        dbus.read_all();

        vbus.read_all();        // updating cached values
        bus.read_all();

        int d = dbus.get<3>();  // reads cached value (the value of pin2 is never updated)
        float a = abus.get<1>();

        int c0, c1, c2, c3, c4, c5;
        float t1;
        int v0, v1, v3, v5;
        float v2, v4;
        //auto l = vbus.get(2);
        std::tie(c0, c2, c3) = bus.read<0, 2, 3>();
        bus.read<0, 1, 3>(c0, t1, c2);
        vbus.read_all(v0, v1, v2, v3, v4, v5);
        auto vd = vbus.get<3>();  // int
        auto va = vbus.get<4>();  // float
        // deducing the type from variadic bus initialization

        int d2 = digit;         // reads cached value for single entry

        // float a = abus[4];           // no bound checking, undefined behaviour
        // float a2 = abus.get<4>();    // compile time error: array index out of bound
    }
}


// kinda more useful example

void calculate(const Cached::DBus<4> &bus) {
    // ...
}

void calculate1(const Cached::DBus<4> &bus) {
    // ...
}

int example_main2()
{
    Cached::DBus<4> dbus {pin1, pin2, pin3, pin4}; 
    
    while (true) {
        dbus.read_all();  // real read only once in a cycle 
        calculate(dbus);
        calculate1(dbus); // using cached values many tymes
    }
}