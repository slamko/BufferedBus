#ifndef CACHE_BUS_H
#define CACHE_BUS_H

#include "mbed.h"
#include <mstd_tuple>
#include <mstd_functional>
#include <mstd_type_traits>
#include <initializer_list>

namespace Cached {
    #define OUT_OF_BOUNDS_ERROR "error: Bus index out of bounds"

    template <class In, class Data>
    class InputRead {
    public:
        virtual Data read(bool inverse_read) = 0;

        operator Data();

        Data read_cached() {
            return data;
        }

        InputRead(In& input);
        InputRead(const InputRead &) = delete;
        InputRead& operator =(const InputRead&) = delete; 

        InputRead(InputRead &&) = default;
        
    protected:
        mstd::reference_wrapper<In> input;
        Data data;
    };

    class Digital : public InputRead<DigitalIn, int> {
    public:
        using InputRead::InputRead;
        int read(bool inverse_read = false) override;
    };

    class Analog : public InputRead<AnalogIn, float> {
    public:
        using InputRead::InputRead;
        float read(bool inverse_read = false) override;
    };

    template<class T, size_t N>
    class Bus : private NonCopyable<Bus<T, N>> {
    private:
        T list[N];

    public:
        template <class ...PT>
        Bus(PT&& ...list);

        template <size_t I>
        auto get();

        auto operator [](size_t index);

        void read_all(bool inverse_read = false);

        template <size_t I>
        void read(bool inverse_read = false);

        template <size_t I, size_t In, size_t ...Index>
        void read(bool inverse_read = false);

        void read(mstd::initializer_list<size_t> ids, bool inverse_read = false);
    };

    template <class In, class Data>
    InputRead<In, Data>::InputRead(In &input) : input(input) {}

    template <class In, class Data>
    InputRead<In, Data>::operator Data() {
        return data;
    }

    template <class T, size_t N>
    template <class ...PT>
    Bus<T, N>::Bus(PT&& ...list) : list {list...} {}

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
    void Bus<T, N>::read(mstd::initializer_list<size_t> ids, bool inverse_read) {
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

    template <size_t N>
    using void_if_non_nil = typename mstd::enable_if_t<N != 0, void>;

    template <size_t N>
    using void_if_nil = typename mstd::enable_if_t<N == 0, void>;

    template <class T>
    struct assoc_data_type;

    template <>
    struct assoc_data_type<Digital> : mstd::type_identity<int> {};

    template <>
    struct assoc_data_type<Analog> : mstd::type_identity<float> {};

    template <class T>
    using assoc_data_type_t = typename assoc_data_type<mstd::remove_reference_t<T>>::type;

    template<class ...T>
    class VBus {
    private:
        mstd::tuple<T...> list;
        bool _inverse_read;

        template <size_t I>
        using list_index_data = assoc_data_type_t<decltype(mstd::get<I>(list))>;

        template <size_t ...Ids>
        using bound_data_bus = mstd::tuple<assoc_data_type_t<decltype(mstd::get<Ids>(list))>...>;

    public:
        template <class ...PT>
        VBus(PT &&...list);
        
        template <class ...PT>
        VBus(bool inverse_reading, PT &&...list);

        VBus(const VBus &) = delete;
        VBus& operator =(const VBus &) = delete;
        
        VBus(VBus &&vbus) = default;
        VBus& operator =(VBus &&vbus) = default;
        
        using data_bus = mstd::tuple<assoc_data_type_t<T>...>;

        template <size_t I>
        auto get() -> list_index_data<I>;
        
        template <size_t I>
        auto read() -> list_index_data<I>;

        template <size_t I>
        auto read(bool inverse_read) -> list_index_data<I>;

        template <size_t I, size_t In, size_t ...Index>
        bound_data_bus<I, In, Index...> read();

        template <size_t ...Index>
        bound_data_bus<Index...> read(bool inverse_read);

        data_bus read_all();

        data_bus read_all(bool inverse_read);

        template <size_t ...Index, class ...DataArgs>
        void read(DataArgs &...dargs);

        template <class ...DataArgs>
        void read_all(DataArgs &... dargs);

        bool set_inverse_read(bool inverse_read) {
            bool _inv = this->_inverse_read;
            this->_inverse_read = inverse_read;
            return _inv;
        }

    private:
        template <size_t I>
        void_if_nil<I - 1U> read_all_iter(data_bus &data);

        template <size_t I>
        void_if_non_nil<I - 1U> read_all_iter(data_bus &data);

        template <class DBus, size_t I>
        void read_iter(DBus &data);

        template <class DBus, size_t I, size_t In, size_t ...Index>
        void read_iter(DBus &data);
    };

    template <class ...T>
    template <class ...PT>
    VBus<T...>::VBus(PT &&...list) : 
        list {list...}, _inverse_read {false} {}

    template <class ...T>
    template <class ...PT>
    VBus<T...>::VBus(bool inverse_read, PT &&...list) : 
        list {list...}, _inverse_read {inverse_read} {}

    template <class ...T>
    template <size_t I>
    auto VBus<T...>::get() -> list_index_data<I> {
        return mstd::get<I>(list).read_cached();
    }

    template <class ...T>
    template <size_t I>
    auto VBus<T...>::read() -> list_index_data<I> {
        return mstd::get<I>(list).read(_inverse_read);
    }

    template <class ...T>
    template <size_t I>
    auto VBus<T...>::read(bool inverse_read)-> list_index_data<I> {
        bool _inv = set_inverse_read(inverse_read);
        list_index_data<I> val = read<I>();
        set_inverse_read(_inv);
        return val;
    }

    template <class ...T>
    template <class DBus, size_t I>
    void VBus<T...>::read_iter(DBus &data) {
        mstd::get<0>(data) = read<I>();
    }

    template <class ...T>
    template <class DBus, size_t I, size_t In, size_t ...Index>
    void VBus<T...>::read_iter(DBus &data) {
        auto read_val = mstd::get<I>(list).read();
        mstd::get<sizeof...(Index) + 1U>(data) = read_val;
        read_iter<DBus, In, Index...>(data);
    }

    template <class ...T>
    template <size_t I, size_t In, size_t ...Index>
    auto VBus<T...>::read() -> bound_data_bus<I, In, Index...> {
        bound_data_bus<I, In, Index...> dbus;
        read_iter<decltype(dbus), I, In, Index...>(dbus);
        return dbus;
    }

    template <class ...T>
    template <size_t ...Index>
    auto VBus<T...>::read(bool inverse_read) -> bound_data_bus<Index...> {
        bool _inv = set_inverse_read(inverse_read);
        bound_data_bus<Index...> dbus = read<Index...>();
        set_inverse_read(_inv);
        return dbus;
    }

    template <class ...T>
    template <size_t ...Indexes, class ...DataArgs>
    void VBus<T...>::read(DataArgs &...args) {
        mstd::tie(args...) = read<Indexes...>();
    }

    template <class ...T>
    template <size_t I>
    void_if_nil<I - 1U> VBus<T...>::read_all_iter(data_bus &data) {
        auto read_val = read<0>();
        mstd::get<0>(data) = read_val;
    }
  
    template <class ...T>
    template <size_t I> 
    void_if_non_nil<I - 1U> VBus<T...>::read_all_iter(data_bus &data) {
        auto read_val = read<I>();
        mstd::get<I>(data) = read_val;
        read_all_iter<I - 1U>(data);
    }

    template <class ...T>
    auto VBus<T...>::read_all() -> data_bus {
        data_bus dbus;
        read_all_iter<mstd::tuple_size<decltype(list)>::value - 1U>(dbus);
        return dbus;
    }

    template <class ...T>
    auto VBus<T...>::read_all(bool inverse_read) -> data_bus {
        bool _inv = set_inverse_read(inverse_read);
        data_bus dbus = read_all();
        set_inverse_read(_inv);
        return dbus;
    }

    template <class ...T>
    template <class ...DataArgs>
    void VBus<T...>::read_all(DataArgs &...dargs) {
        mstd::tie(dargs...) = read_all();
    }

    template <class T>
    struct assoc_type;

    template <>
    struct assoc_type<DigitalIn> : mstd::type_identity<Digital> {};

    template <>
    struct assoc_type<AnalogIn> : mstd::type_identity<Analog> {};

    template <class T>
    using assoc_type_t = typename assoc_type<T>::type;

    template <class ...Args>
    VBus<assoc_type_t<Args>...> make_vbus(bool inverse_read = false, Args &&...args) {
        return VBus<assoc_type_t<Args>...>(inverse_read, args...);
    }
}

#endif // CACHE_BUS_H