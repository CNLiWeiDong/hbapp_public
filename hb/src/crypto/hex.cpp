#include <hb/crypto/cryptopp.h>
#include <hb/crypto/hex.h>

#include <boost/algorithm/string.hpp>
#include <string>

namespace hb::crypto {

    using namespace CryptoPP;

    std::string hex(const std::string &str) {
        std::string encoded;
        StringSource ss(str, true, new HexEncoder(new StringSink(encoded)));
        return encoded;
    }

    std::string hex_to_str(const std::string &hex_str) {
        std::string decoded;
        StringSource ssk(hex_str, true /*pump all*/,
                         new HexDecoder(new StringSink(decoded))  // HexDecoder
        );
        return decoded;
    }

    int64_t hex_to_dec(std::string hex_str) {
        if (boost::istarts_with(hex_str, "0x")) {  // 不区分大小写
            hex_str = std::string(hex_str.begin() + 2, hex_str.end());
        }
        int64_t n;
        std::istringstream(hex_str) >> std::hex >> n;
        return n;
    }

    //十进制数字转十六进字符串
    std::string dec_to_hex(int64_t decn) {
        std::stringstream ss;
        ss << std::hex << std::showbase << std::nouppercase << decn;
        std::string hex_str = ss.str();
        return hex_str;
    }

    int oct_to_dec(std::string octstr) {
        int decn;
        std::stringstream ss;
        ss.str(octstr);
        ss << std::oct << std::showbase << std::nouppercase << decn;
        return decn;
    }
}  // namespace hb::crypto
