#include "mbed.h"

namespace Buffered {
    template <class In, class Data>
    class InputRead {
    public:
        virtual void read(bool inverse_read) = 0;
        operator Data();
        InputRead(In& in);

        InputRead(const InputRead&) = delete;
        InputRead& operator =(const InputRead&) = delete;
        
    protected:
        In& in;
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
        T list[N];
    public:
        template <class... PT>
        Bus(PT &... list);
        template <size_t I>
        T& get();
        T& operator [](size_t index);
        void read(bool inverse_read = false);
    };

    template <class In, class Data>
    InputRead<In, Data>::InputRead(In &in) : in(in) {}

    template <class In, class Data>
    InputRead<In, Data>::operator Data() {
        return data;
    }

    template <class T, size_t N>
    template <class... PT>
    Bus<T, N>::Bus(PT &... list) : list {list...} {}

    template <class T, size_t N>
    T& Bus<T, N>::operator [](size_t index) {
        return this->list[index];
    }

    template <class T, size_t N>
    void Bus<T, N>::read(bool inverse_read) {
        for (auto in : list) {
            in.read(inverse_read);
        }
    }

    template <class T, size_t N>
    template <size_t I>
    T& Bus<T, N>::get() {
        static_assert(I < N, "out of bound");

        return list[I];
    }

    template <size_t N>
    using DBus = Bus<Digital, N>;

    template <size_t N>
    using ABus = Bus<Analog, N>;
}