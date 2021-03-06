#include <hb/crypto/cryptopp.h>
#include <hb/crypto/hex.h>
#include <hb/crypto/md5.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace hb {
    namespace crypto {

        using namespace CryptoPP;

        std::string md5(const std::string &msg) {
            std::string digest;

            MD5 hash;
            hash.Update((const byte *)msg.data(), msg.size());
            digest.resize(hash.DigestSize());
            hash.Final((byte *)&digest[0]);

            return digest;
        }
        std::string md5_hex(const std::string &msg) {
            std::string h = hex(md5(msg));
            boost::to_lower(h);
            return h;
        }
        bool md5_verify(const std::string &msg, const std::string &digest) {
            MD5 hash;
            return hash.VerifyDigest((byte *)digest.c_str(), (const byte *)msg.c_str(), msg.size());
        }

    }  // namespace crypto
}  // namespace hb
