#include "Hashing.h"

namespace Hashing {
std::string hexStr(const uint8_t* data, int len)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (int i(0); i < len; ++i)
        ss << std::setw(2) << (int)data[i];

    return ss.str();
}

std::string HashFileMD5(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    EVP_MD_CTX* md5Context = EVP_MD_CTX_new();
    EVP_MD_CTX_init(md5Context);
    EVP_DigestInit_ex(md5Context, EVP_md5(), nullptr);

    const size_t bufferSize = 4096;
    char buffer[bufferSize];

    while (!file.eof()) {
        file.read(buffer, bufferSize);
        EVP_DigestUpdate(md5Context, buffer, file.gcount());
    }

    std::array<uint8_t, 16> result;
    EVP_DigestFinal_ex(md5Context, result.data(), nullptr);
    file.close();

    EVP_MD_CTX_free(md5Context);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (const uint8_t i : result) {
        ss << std::setw(2) << (int)i;
    }

    return ss.str();
}
}
