#pragma once

#include <string>

namespace hb {
    namespace crypto {
        std::string cfb_aes_encrypt(const std::string &sKey, const std::string &plainText);
        std::string cfb_aes_encrypt(const std::string &plainText);
        std::string cfb_aes_decrypt(const std::string &sKey, const std::string &plainText);
        std::string cfb_aes_decrypt(const std::string &plainText);
    }  // namespace crypto
}  // namespace hb
