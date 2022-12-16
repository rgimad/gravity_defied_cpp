#pragma once

#include <algorithm>
#include <iostream>

namespace Stream {
    template <class T>
    void readVariable(T *p, std::istream &is, bool swapEndianness = false, std::size_t size = 0) {
        char *pChar = reinterpret_cast<char*>(p);
        if (!size) {
            size = sizeof(T);
        }
        is.read(pChar, size);
        if (swapEndianness) {
            std::reverse(pChar, pChar + size);
        }
    }

    template <class T>
    void writeVariable(T *p, std::ostream &os, std::size_t size = 0) {
        char *pChar = reinterpret_cast<char*>(p);
        if (!size) {
            size = sizeof(T);
        }
        os.write(pChar, size);
    }
};