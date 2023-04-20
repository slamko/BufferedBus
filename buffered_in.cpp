#include "mbed.h"
#include <vector>
#include <initializer_list>
#include "buf_bus.h"

namespace Buffered {
    void Digital::read(bool inverse_read) {
        if (inverse_read) {
            data = !in.read();
        } else {
            data = in.read();
        }
    }

    void Analog::read(bool inverse_read) {
        if (inverse_read) {
            data = (1.0f - in.read());
        } else {
            data = in.read();
        }
    }
}