#include "mbed.h"
#include "cache_bus.h"

namespace Cached {
    int Digital::read(bool inverse_read) {
        if (inverse_read) {
            data = !input.get().read();
        } else {
            data = input.get().read();
        }
        return data;
    }

    float Analog::read(bool inverse_read) {
        if (inverse_read) {
            data = (1.0f - input.get().read());
        } else {
            data = input.get().read();
        }
        return data;
    }
}