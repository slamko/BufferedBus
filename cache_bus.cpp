#include "mbed.h"
#include <vector>
#include <initializer_list>
#include "cache_bus.h"

namespace Cached {
    int Digital::read(bool inverse_read) {
        if (inverse_read) {
            data = !in.get().read();
        } else {
            data = in.get().read();
        }
        return data;
    }

    float Analog::read(bool inverse_read) {
        if (inverse_read) {
            data = (1.0f - in.get().read());
        } else {
            data = in.get().read();
        }
        return data;
    }
}