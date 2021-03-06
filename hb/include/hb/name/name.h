#pragma once
#include <boost/algorithm/string.hpp>
#include <string>

namespace hb::name {
    using std::string;

    static constexpr uint64_t char_to_symbol(char c) {
        if (c >= 'a' && c <= 'z') return (c - 'a') + 6;
        if (c >= '1' && c <= '5') return (c - '1') + 1;
        return 0;
    }

    // Each char of the string is encoded into 5-bit chunk and left-shifted
    // to its 5-bit slot starting with the highest slot for the first char.
    // The 13th char, if str is long enough, is encoded into 4-bit chunk
    // and placed in the lowest 4 bits. 64 = 12 * 5 + 4
    static constexpr uint64_t string_to_name(const char* str) {
        uint64_t name = 0;
        int i = 0;
        for (; str[i] && i < 12; ++i) {
            // NOTE: char_to_symbol() returns char type, and without this explicit
            // expansion to uint64 type, the compilation fails at the point of usage
            // of string_to_name(), where the usage requires constant (compile time) expression.
            name |= (char_to_symbol(str[i]) & 0x1f) << (64 - 5 * (i + 1));
        }

        // The for-loop encoded up to 60 high bits into uint64 'name' variable,
        // if (strlen(str) > 12) then encode str[12] into the low (remaining)
        // 4 bits of 'name'
        if (i == 12) name |= char_to_symbol(str[12]) & 0x0F;
        return name;
    }

    __attribute__((unused)) static string name_to_string(uint64_t value) {
        static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";
        string str(13, '.');
        uint64_t tmp = value;
        for (uint32_t i = 0; i <= 12; ++i) {
            char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
            str[12 - i] = c;
            tmp >>= (i == 0 ? 4 : 5);
        }

        boost::algorithm::trim_right_if(str, [](char c) { return c == '.'; });
        return str;
    }
}  // namespace hb::name

#define SN(X) hb::name::string_to_name(#X)
#define NS(X) hb::name::name_to_string(#X)
