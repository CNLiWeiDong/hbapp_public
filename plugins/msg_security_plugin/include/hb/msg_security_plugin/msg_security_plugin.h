//
// Created by 李卫东 on 2019-02-26.
//
#pragma once

#include <hb/msg_security_plugin/msg_security_plugin_impl.h>
#include <hb/log_plugin/log_plugin.h>
#include <appbase/application.hpp>
#include <functional>
#include <memory>
#include <string>
#include <thread>

using namespace std;

namespace hb {
    namespace plugin {
        using namespace appbase;
        class msg_security_plugin : public appbase::plugin<msg_security_plugin> {
          public:
            APPBASE_PLUGIN_REQUIRES((log_plugin))
            msg_security_plugin();
            virtual ~msg_security_plugin();
            virtual void set_program_options(options_description &, options_description &) override;

            void plugin_initialize(const variables_map &);
            void plugin_startup();
            void plugin_shutdown();

            void encrypt(const string &msg, string &chiper, string &encrypted_msg) {
              my->encrypt(msg, chiper, encrypted_msg);
            }
            void decrypt(const string &encrypted_msg, const string &chiper, string &msg){
               my->decrypt(encrypted_msg, chiper, msg);
            }

          private:
            shared_ptr<msg_security_plugin_impl> my;
        };
    }  // namespace plugin
}  // namespace hb