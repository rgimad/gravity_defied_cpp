#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>

namespace Helpers {
    template <class T>
    void readVariableFromStream(T *p, std::istream &is, bool swapEndianness = false, std::size_t size = 0) {
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
    void writeVariableToStream(T *p, std::ostream &os, std::size_t size = 0) {
        char *pChar = reinterpret_cast<char*>(p);
        if (!size) {
            size = sizeof(T);
        }
        os.write(pChar, size);
    }

    template <class T>
    std::string join(const std::vector<T> &vector, const std::string &delimiter) {
        if (vector.empty()) {
            return "";
        }

        std::string result;

        for (auto i = vector.cbegin(); i != vector.cend(); i++) {
            if constexpr (std::is_same<T, std::string>::value) {
                result += *i;
            } else {
                result += std::to_string(*i);
            }

            if (i != vector.cend() - 1) {
                result += delimiter;
            }
        }

        return result;
    }
};