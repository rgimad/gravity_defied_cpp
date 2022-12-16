#pragma once

namespace String {
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