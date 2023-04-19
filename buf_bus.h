#include "mbed.h"
#include <vector>
#include <array>
#include <initializer_list>

namespace Buffered {
    template <class In, class Data>
    class InputRead {
    public:
        virtual void read(bool inverse_read) = 0;
        operator Data();
        InputRead(In in);
    protected:
        In in;
        Data data;
    };

    class Digital : public InputRead<DigitalIn, int> {
    public:
        using InputRead::InputRead;
        void read(bool inverse_read) override;
    };

    class Analog : public InputRead<AnalogIn, float> {
    public:
        using InputRead::InputRead;
        void read(bool inverse_read) override;
    };

    template<class T>
    class Bus {
    private:
        std::vector<T> list;
    public:
        Bus(std::initializer_list<T> list);
        void read();
    };
}