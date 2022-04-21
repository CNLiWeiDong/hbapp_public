//
// Created by 李卫东 on 2019-02-26.
//

#pragma once

#include <hb/hb.h>

#include <appbase/application.hpp>
#include <memory>

namespace hb::plugin {
    using namespace std;
    using namespace appbase;

    class msg_security_plugin_impl : public boost::noncopyable {
      public:
        msg_security_plugin_impl() {}
        void start();
        void http_security_private_key(const string &value) { _http_security_private_key = value; }
        void http_security_public_key(const string &value) { _http_security_public_key = value; }

        void encrypt(const string &msg, string &chiper, string &encrypted_msg);
        void decrypt(const string &encrypted_msg, const string &chiper, string &msg);

      private:
        string _http_security_private_key;
        string _http_security_public_key;
    };
}  // namespace hb::plugin