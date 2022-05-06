#pragma once
#include <string>

namespace hb::crypto {

    std::string hex(const std::string &str);
    std::string hex_to_str(const std::string &hex_str);
    int64_t hex_to_dec(std::string hex_str);
    std::string dec_to_hex(int64_t decn);
}  // namespace hb::crypto
