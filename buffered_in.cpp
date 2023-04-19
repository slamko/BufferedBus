#include "mbed.h"
#include <vector>
#include <initializer_list>

namespace Buffered {
    class Digital {
    private:
        DigitalIn in;

    public:
        Digital(DigitalIn in) : in(in) {

        }
    };

    class Bus {
    private:
        std::vector<Digital> list;
    public:
        Bus(std::initializer_list<Digital> list) : list(list) {}
    };
}