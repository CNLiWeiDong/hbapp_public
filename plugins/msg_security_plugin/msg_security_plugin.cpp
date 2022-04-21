//
// Created by 李卫东 on 2019-02-26.
//

#include <hb/msg_security_plugin/msg_security_plugin.h>
#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <string>

namespace hb {
    namespace plugin {
        static appbase::abstract_plugin &_msg_security_plugin
            = app().register_plugin<msg_security_plugin>();
        msg_security_plugin::msg_security_plugin() {}
        msg_security_plugin::~msg_security_plugin() {}

        void msg_security_plugin::set_program_options(options_description &cli,
                                                      options_description &cfg) {
            cfg.add_options()("http-security-rsa-private-key",
                              boost::program_options::value<string>()->default_value(
                                  "http-security-rsa-private-key.pem"),
                              "rsa private key use to encrypt http body!")(
                "http-security-rsa-private-pass",
                boost::program_options::value<string>()->default_value(""),
                "the password of rsa private key!")(
                "http-security-rsa-public-key",
                boost::program_options::value<string>()->default_value(
                    "http-security-rsa-public-key.pem"),
                "rsa public key use to decrypt http body!");
        }
        void msg_security_plugin::plugin_initialize(const variables_map &options) {
            log_info << "msg_security_plugin::plugin_initialize";
            hb_try {
                my = make_shared<msg_security_plugin_impl>();
                auto private_key_file
                    = app().config_dir() / options.at("http-security-rsa-private-key").as<string>();
                my->http_security_private_key(hb::crypto::read_rsa_pem_pri(
                    private_key_file.c_str(),
                    options.at("http-security-rsa-private-pass").as<string>()));

                auto public_key_file
                    = app().config_dir() / options.at("http-security-rsa-public-key").as<string>();
                my->http_security_public_key(hb::crypto::read_rsa_pem_pub(public_key_file.c_str()));
            }
            hb_catch([&](const auto &e) {
                log_throw("msg_security_plugin::plugin_initialize", e);
                std::exit(-2);
            });
        }
        void msg_security_plugin::plugin_startup() {
            log_info << "msg_security_plugin::plugin_startup";
        }
        void msg_security_plugin::plugin_shutdown() {
            log_info << "msg_security_plugin::plugin_shutdown";
        }
    }  // namespace plugin
}  // namespace hb