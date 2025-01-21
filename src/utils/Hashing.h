#pragma once

#include <cstring>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <array>

#include <openssl/md5.h>
#include <openssl/evp.h>

namespace Hashing {
    std::string HashFileMD5(const std::string& filename);
};