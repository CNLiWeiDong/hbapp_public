//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <hb/log/log.h>

#include <appbase/application.hpp>
#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace hb::plugin {
      using namespace std;
      using namespace appbase;
      class send_mail_plugin_impl : public enable_shared_from_this<send_mail_plugin_impl> {
        
        public:
          void set_host(const string& host) { _host = host; }
          void set_port(const string& port) { _port = port; }
          void set_user(const string& user) { _user = user; }
          void set_pass(const string& pass) { _pass = pass; }

        public:
          send_mail_plugin_impl() {}
          ~send_mail_plugin_impl() {}
          void send_mail(const string &to, const string &subject, const string &content);
        private:
          string _host{};
          string _port{};
          string _user{};
          string _pass{};
          // mutex signals_mutex_;
      };
}  // namespace hb
