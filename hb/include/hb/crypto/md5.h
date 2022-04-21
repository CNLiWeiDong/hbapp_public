#pragma once

#include <string>

namespace hb {
    namespace crypto {
        std::string md5(const std::string &msg);
        bool md5_verify(const std::string &msg, const std::string &digest);
        std::string md5_hex(const std::string &msg);
    }  // namespace crypto
}  // namespace hb
