#pragma once

#include <hb/crypto/crypto_error.h>
#include <hb/error/exception.h>

namespace hb {
    namespace crypto {
        using namespace hb::error;

        class hb_crypto_exception : public hb_exception<hb_crypto_exception, SN("crypto")> {
          public:
            hb_crypto_exception() : hb_exception<hb_crypto_exception, SN("crypto")>() {}
        };
    }  // namespace crypto
}  // namespace hb