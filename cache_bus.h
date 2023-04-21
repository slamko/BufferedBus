#ifndef CACHE_VAR_VBus_H
#define CACHE_VAR_VBus_H

#include "mbed.h"
#include <tuple>

namespace Cached {
    #define OUT_OF_BOUNDS_ERROR "error: VBus index out of bounds"

    template <class In, class Data>
    class InputRead {
    public:
        virtual void read(bool inverse_read) = 0;

        operator Data();

        Data read_cached() {
            return data;
        }

        InputRead(In& in);
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
    class Bus : private NonCopyable<Bus<T, N>> {
    private:
        T list[N];

    public:
        template <class... PT>
        Bus(PT &... list);

        template <size_t I>
        auto get();

        auto operator [](size_t index);

        void read_all(bool inverse_read = false);

        template <size_t I>
        void read(bool inverse_read = false);

        template <size_t I, size_t In, size_t ...Index>
        void read(bool inverse_read = false);

        void read(std::initializer_list<size_t> ids, bool inverse_read = false);
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
    auto Bus<T, N>::operator [](size_t index) {
        if (index >= N) {
            #if __cpp_exceptions 
            //    throw OUT_OF_BOUNDS_ERROR;
            #endif
        }
        
        return this->list[index].read_cached();
    }

    template <class T, size_t N>
    void Bus<T, N>::read_all(bool inverse_read) {
        for (auto &in : list) {
            in.read(inverse_read);
        }
    }

    template <class T, size_t N>
    template <size_t I>
    auto Bus<T, N>::get() {
        static_assert(I < N, OUT_OF_BOUNDS_ERROR);

        return list[I].read_cached();
    }

    template <class T, size_t N>
    template <size_t I>
    void Bus<T, N>::read(bool inverse_read) {
        static_assert(I < N, OUT_OF_BOUNDS_ERROR);
        this->list[I].read(inverse_read);
    }

    template <class T, size_t N>
    template <size_t I, size_t In, size_t ...Index>
    void Bus<T, N>::read(bool inverse_read) {
        static_assert(I < N, OUT_OF_BOUNDS_ERROR);
        this->list[I].read(inverse_read);
        read<In, Index...>();
    }

    template <class T, size_t N>
    void Bus<T, N>::read(std::initializer_list<size_t> ids, bool inverse_read) {
        for (auto id : ids) {
            if (id >= N) {
                #if __cpp_exceptions 
                //    throw OUT_OF_BOUNDS_ERROR;
                #endif
            }

            this->list[id].read(inverse_read);
        }
    }

    template <size_t N>
    using DBus = Bus<Digital, N>;

    template <size_t N>
    using ABus = Bus<Analog, N>;


    template<class ...T>
    class VBus : private NonCopyable<VBus<T...>> {
    private:
        std::tuple<T...> list;

    public:
        template <class ...PT>
        VBus(PT &... list);

        template <size_t I>
        auto get();

        void read_all(bool inverse_read = false);

        template <size_t I>
        void read(bool inverse_read = false);

        template <size_t I, size_t In, size_t ...Index>
        void read(bool inverse_read = false);
    };

    template <class ...T>
    template <class ...PT>
    VBus<T...>::VBus(PT &...list) : list {list...} {}

    template <class ...T>
    void VBus<T...>::read_all(bool inverse_read) {
        read<std::tuple_size<decltype(list)>::value - 1>();
    }

    template <class ...T>
    template <size_t I>
    auto VBus<T...>::get() {
        return std::get<I>(list).read_cached();
    }

    template <class ...T>
    template <size_t I>
    void VBus<T...>::read(bool inverse_read) {
        std::get<I>(list).read(inverse_read);
    }

    template <class ...T>
    template <size_t I, size_t In, size_t ...Index>
    void VBus<T...>::read(bool inverse_read) {
        std::get<I>(list).read(inverse_read);
        read<In, Index...>();
    }
}

#endif // CACHE_VAR_VBus_H