#include <hb/crypto/aes.h>
#include <hb/crypto/cryptopp.h>

namespace hb {
    namespace crypto {

        using namespace CryptoPP;

        /*
        (1)AES在密码学中又称Rijndael加密法，是美国联邦政府采用的一种区块加密标准。2006年，高级加密标准已然成为对称密钥加密中最流行的算法之一。
        (2)AES加密数据块分组长度必须为128比特，密钥长度可以是128比特、192比特、256比特中的任意一个。(8比特==1字节)
        (3)在CBC、CFB、OFB CTR模式下除了密钥外，还需要一个初始化向Ⅳ。(ECB模式不用IV)
        */
        std::string cfb_aes_encrypt(const std::string &sKey, const std::string &plainText) {
            std::string outstr;

            //填key
            SecByteBlock key(AES::DEFAULT_KEYLENGTH);
            memset(key, 0x30, key.size());
            sKey.size() <= AES::DEFAULT_KEYLENGTH
                ? memcpy(key, sKey.c_str(), sKey.size())
                : memcpy(key, sKey.c_str(), AES::DEFAULT_KEYLENGTH);

            //填iv
            byte iv[AES::BLOCKSIZE];
            memset(iv, 0x30, AES::BLOCKSIZE);

            AES::Encryption aesEncryption((byte *)key, AES::DEFAULT_KEYLENGTH);

            CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, iv);

            StreamTransformationFilter cfbEncryptor(cfbEncryption,
                                                    new HexEncoder(new StringSink(outstr)));
            cfbEncryptor.Put((byte *)plainText.data(), plainText.size());
            cfbEncryptor.MessageEnd();

            return outstr;
        }

        std::string cfb_aes_encrypt(const std::string &plainText) {
            return cfb_aes_encrypt("", plainText);
        }

        std::string cfb_aes_decrypt(const std::string &sKey, const std::string &plainText) {
            std::string outstr;

            //填key
            SecByteBlock key(AES::DEFAULT_KEYLENGTH);
            memset(key, 0x30, key.size());
            sKey.size() <= AES::DEFAULT_KEYLENGTH
                ? memcpy(key, sKey.c_str(), sKey.size())
                : memcpy(key, sKey.c_str(), AES::DEFAULT_KEYLENGTH);

            //填iv
            byte iv[AES::BLOCKSIZE];
            memset(iv, 0x30, AES::BLOCKSIZE);

            CFB_Mode<AES>::Decryption cfbDecryption((byte *)key, AES::DEFAULT_KEYLENGTH, iv);

            HexDecoder decryptor(
                new StreamTransformationFilter(cfbDecryption, new StringSink(outstr)));
            decryptor.Put((byte *)plainText.data(), plainText.size());
            decryptor.MessageEnd();

            return outstr;
        }
        std::string cfb_aes_decrypt(const std::string &plainText) {
            return cfb_aes_decrypt("", plainText);
        }

    }  // namespace crypto
}  // namespace hb
