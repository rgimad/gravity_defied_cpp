#pragma once

#include <algorithm>

namespace Helpers {
    template <class T>
    void readVariableFromFileStream(T *p, std::ifstream &is, bool swapEndianness = false, std::size_t size = 0) {
        char *pChar = reinterpret_cast<char*>(p);
        if (!size) {
            size = sizeof(T);
        }
        is.read(pChar, size);
        if (swapEndianness) {
            std::reverse(pChar, pChar + size);
        }
    }
};