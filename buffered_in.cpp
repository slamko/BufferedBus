#include "mbed.h"
#include <vector>
#include <initializer_list>
#include "buf_bus.h"

namespace Buffered {
    template <class In, class Data>
    InputRead<In, Data>::InputRead(In in) : in(in) {}

    template <class In, class Data>
    InputRead<In, Data>::operator Data() {
        return data;
    }

    template <class T>
    Bus<T>::Bus(std::initializer_list<T> list) : list(list) {}

    void Digital::read(bool inverse_read = false) {
        if (inverse_read) {
            data = !in.read();
        } else {
            data = in.read();
        }
    }

    void Analog::read(bool inverse_read = false) {
        if (inverse_read) {
            data = (1.0f - in.read());
        } else {
            data = in.read();
        }
    }
}