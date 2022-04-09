#pragma once

#include <string>

namespace hb {
    namespace crypto {
        std::string random_str(unsigned len);
        unsigned random_num(unsigned max);

    }  // namespace crypto
}  // namespace hb
