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
        void read(bool inverse_read = false) override;
    };

    class Analog : public InputRead<AnalogIn, float> {
    public:
        using InputRead::InputRead;
        void read(bool inverse_read = false) override;
    };

    template<class T, size_t N>
    class Bus {
    private:

    public:
        std::array<T, N> list;

        template <class... PT>
        Bus(PT... list);
        template <size_t I>
        T& get();
        T& operator [](size_t index);
        void read();
    };


    template <class In, class Data>
    InputRead<In, Data>::InputRead(In in) : in(in) {}

    template <class In, class Data>
    InputRead<In, Data>::operator Data() {
        return data;
    }

    template <class T, size_t N>
    template <class... PT>
    Bus<T, N>::Bus(PT... list) : list {list...} {}

    template <class T, size_t N>
    T& Bus<T, N>::operator [](size_t index) {
        return this->list.at(index);
    }

    template <class T, size_t N>
    void Bus<T, N>::read() {
        for (auto in : list) {
            in.read();
        }
    }

    template <class T, size_t N>
    template <size_t I>
    T& Bus<T, N>::get() {
        return std::get<I>(list);
    }
}