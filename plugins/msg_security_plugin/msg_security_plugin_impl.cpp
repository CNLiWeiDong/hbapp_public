//
// Created by 李卫东 on 2019-02-26.
//

#include <hb/log_plugin/log_plugin.h>
#include <hb/msg_security_plugin/msg_security_plugin_impl.h>
#include <hb/msg_security_plugin/smg_security_error.h>

namespace hb::plugin {

    void msg_security_plugin_impl::start() {}
    // return chiper and encrypted_msg
    void msg_security_plugin_impl::encrypt(const string &msg, string &chiper,
                                           string &encrypted_msg) {
        try {
            string chiper_code = hb::crypto::random_str(10);
            string chiper_md5 = hb::crypto::md5_hex(chiper_code);
            chiper = hb::crypto::rsa_encrypt(_http_security_public_key, chiper_code);
            encrypted_msg = hb::crypto::cfb_aes_encrypt(chiper_md5, msg);
        } catch (...) {
            hb_throw(smg_security_error().msg("msg_security_plugin_impl encrypt msg error."));
        }
    }

    // return msg
    void msg_security_plugin_impl::decrypt(const string &encrypted_msg, const string &chiper,
                                           string &msg) {
        try {
            string chiper_code = hb::crypto::rsa_decrypt(_http_security_private_key, chiper);
            string chiper_md5 = hb::crypto::md5_hex(chiper_code);
            msg = hb::crypto::cfb_aes_decrypt(chiper_md5, encrypted_msg);
        } catch (...) {
            hb_throw(smg_security_error().msg("msg_security_plugin_impl decrypt msg error."));
        }
    }
}  // namespace hb::plugin