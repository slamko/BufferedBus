#include "mbed.h"
#include <vector>
#include <initializer_list>

namespace Buffered {
    template <class In, class Data>
    class InputRead {
    public:
        template <class Ret>
        operator Ret();
        InputRead(In in) : in(in) {}
    protected:
        In in;
        Data data;
    };

    class Digital : public InputRead<DigitalIn, int> {
    public:
        using InputRead::InputRead;
    };

    class Analog : public InputRead<AnalogIn, float> {
    public:
    };

    template<class T>
    class Bus {
    private:
        std::vector<T> list;
    public:
        Bus(std::initializer_list<T> list) : list(list) {}
    };
}