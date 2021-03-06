#include <hb/crypto/base.h>
#include <hb/crypto/cryptopp.h>

namespace hb {
    namespace crypto {

        using namespace CryptoPP;

        std::string base64(const std::string &str) {
            std::string encoded;
            StringSource ss(str, true, new Base64Encoder(new StringSink(encoded)));
            return encoded;
        }

        std::string base64_to_str(const std::string &base64_str) {
            std::string decoded;
            StringSource ssk(base64_str, true /*pump all*/,
                             new Base64Decoder(new StringSink(decoded))  // HexDecoder
            );
            return decoded;
        }

        std::string base64url(const std::string &str) {
            std::string encoded;
            StringSource ss(str, true, new Base64URLEncoder(new StringSink(encoded)));
            return encoded;
        }

        std::string base64url_to_str(const std::string &base64url_str) {
            std::string decoded;
            StringSource ssk(base64url_str, true /*pump all*/,
                             new Base64URLDecoder(new StringSink(decoded))  // HexDecoder
            );
            return decoded;
        }

        std::string base32(const std::string &str) {
            std::string encoded;
            StringSource ss(str, true, new Base32Encoder(new StringSink(encoded)));
            return encoded;
        }

        std::string base32_to_str(const std::string &base32_str) {
            std::string decoded;
            StringSource ssk(base32_str, true /*pump all*/,
                             new Base32Decoder(new StringSink(decoded))  // HexDecoder
            );
            return decoded;
        }
    }  // namespace crypto
}  // namespace hb
