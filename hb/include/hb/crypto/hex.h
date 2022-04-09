#pragma once
#include <string>

namespace hb {
    namespace crypto {

        std::string hex(const std::string &str);
        std::string hex_to_str(const std::string &hex_str);
    }  // namespace crypto
}  // namespace hb
