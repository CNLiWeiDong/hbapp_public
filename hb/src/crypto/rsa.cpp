#include <hb/crypto/cryptopp.h>
#include <hb/crypto/sha.h>

namespace hb {
    namespace crypto {

        using namespace CryptoPP;
        using namespace std;

        std::string pri_to_hex(const RSA::PrivateKey &key) {
            std::string strPri;
            HexEncoder privString(new StringSink(strPri));
            key.DEREncode(privString);
            privString.MessageEnd();
            return strPri;
        }
        std::string pub_to_hex(const RSA::PublicKey &key) {
            std::string strPub;
            HexEncoder pubString(new StringSink(strPub));
            key.DEREncode(pubString);
            pubString.MessageEnd();
            return strPub;
        }
        RSA::PrivateKey hex_to_pri(const std::string &strPri) {
            StringSource privString(strPri, true, new HexDecoder);
            RSA::PrivateKey privateKey;
            privateKey.Load(privString);
            return privateKey;
        }
        RSA::PublicKey hex_to_pub(const std::string &strPub) {
            StringSource pubString(strPub, true, new HexDecoder);
            RSA::PublicKey publicKey;
            publicKey.Load(pubString);
            return publicKey;
        }
        void create_rsa_key(std::string &strPri, std::string &strPub) {
            AutoSeededRandomPool rng;
            // Create Keys
            RSA::PrivateKey privateKey;
            privateKey.GenerateRandomWithKeySize(rng, 3072);
            RSA::PublicKey publicKey(privateKey);

            strPri = pri_to_hex(privateKey);
            strPub = pub_to_hex(publicKey);
        }

        std::string rsa_encrypt(const std::string &strPub, const std::string &plainText) {
            AutoSeededRandomPool rng;
            StringSource pubString(strPub, true, new HexDecoder);
            RSAES_OAEP_SHA_Encryptor pub(pubString);

            std::string result;
            StringSource(
                plainText, true,
                new PK_EncryptorFilter(rng, pub, new Base64Encoder(new StringSink(result))));
            return result;
        }

        std::string rsa_decrypt(const std::string &strPri, const std::string &cipherText) {
            AutoSeededRandomPool rng;
            StringSource privString(strPri, true, new HexDecoder);
            RSAES_OAEP_SHA_Decryptor priv(privString);
            std::string result;
            StringSource(
                cipherText, true,
                new Base64Decoder(new PK_DecryptorFilter(rng, priv, new StringSink(result))));
            return result;
        }

        std::string sign_msg(const std::string &strPri, const std::string &msg) {
            AutoSeededRandomPool rng;
            RSA::PrivateKey privateKey = hex_to_pri(strPri);
            RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

            std::string signature;
            StringSource ss1(msg, true,
                             new SignerFilter(rng, signer,
                                              new StringSink(signature))  // SignerFilter
            );                                                            // StringSource
            return signature;
        }

        void verify_msg(const std::string &strPub, const std::string &msg, const std::string &sig) {
            RSA::PublicKey publicKey = hex_to_pub(strPub);
            RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);
            StringSource ss2(
                msg + sig, true,
                new SignatureVerificationFilter(
                    verifier, NULL,
                    SignatureVerificationFilter::THROW_EXCEPTION)  // SignatureVerificationFilter
            );                                                     // StringSource
        }

        std::string read_rsa_pem_pri(const std::string &pem_path, const std::string &pass) {
            FileSource fs(pem_path.c_str(), true);
            RSA::PrivateKey key;
            if (pass.size() > 0)
                PEM_Load(fs, key, pass.data(), pass.size());
            else
                PEM_Load(fs, key);
            return pri_to_hex(key);
        }
        std::string pem_str_to_rsa_pri(const std::string &pem_str, const std::string &pass) {
            StringSource ss(pem_str, true);
            RSA::PrivateKey key;
            if (pass.size() > 0)
                PEM_Load(ss, key, pass.data(), pass.size());
            else
                PEM_Load(ss, key);
            return pri_to_hex(key);
        }

        std::string read_rsa_pem_pub(const std::string &pem_path) {
            FileSource fs(pem_path.c_str(), true);
            RSA::PublicKey key;
            PEM_Load(fs, key);
            return pub_to_hex(key);
        }
        std::string pem_str_to_rsa_pub(const std::string &pem_str) {
            StringSource ss(pem_str, true);
            RSA::PublicKey key;
            PEM_Load(ss, key);
            return pub_to_hex(key);
        }

        void sava_rsa_pem_pri(const std::string &pem_path, const std::string &strPri,
                              const std::string &pass) {
            AutoSeededRandomPool rng;
            RSA::PrivateKey privateKey = hex_to_pri(strPri);

            FileSink fs(pem_path.c_str(), true);
            if (pass.size() > 0)
                PEM_Save(fs, privateKey, rng, "AES-128-CBC", pass.data(), pass.size());
            else
                PEM_Save(fs, privateKey);
        }

        void sava_rsa_pem_pub(const std::string &pem_path, const std::string &strPub) {
            RSA::PublicKey publicKey = hex_to_pub(strPub);
            FileSink fs(pem_path.c_str(), true);
            PEM_Save(fs, publicKey);
        }

        string rsa_pub_to_pem_str(const std::string &strPub) {
            RSA::PublicKey publicKey = hex_to_pub(strPub);
            std::string pem_str;
            StringSink sk(pem_str);
            PEM_Save(sk, publicKey);
            return pem_str;
        }
        string rsa_pub_to_pem_str(const RSA::PublicKey &publicKey) {
            std::string pem_str;
            StringSink sk(pem_str);
            PEM_Save(sk, publicKey);
            return pem_str;
        }
        string rsa_pri_to_pem_str(const std::string &strPri, const std::string &pass) {
            RSA::PrivateKey privateKey = hex_to_pri(strPri);
            std::string pem_str;
            StringSink sk(pem_str);
            if (pass.size() > 0) {
                AutoSeededRandomPool rng;
                PEM_Save(sk, privateKey, rng, "AES-128-CBC", pass.data(), pass.size());
            } else
                PEM_Save(sk, privateKey);
            return pem_str;
        }
        string rsa_pri_to_pem_str(const RSA::PrivateKey &privateKey, const std::string &pass) {
            std::string pem_str;
            StringSink sk(pem_str);
            if (pass.size() > 0) {
                AutoSeededRandomPool rng;
                PEM_Save(sk, privateKey, rng, "AES-128-CBC", pass.data(), pass.size());
            } else
                PEM_Save(sk, privateKey);
            return pem_str;
        }

    }  // namespace crypto
}  // namespace hb
