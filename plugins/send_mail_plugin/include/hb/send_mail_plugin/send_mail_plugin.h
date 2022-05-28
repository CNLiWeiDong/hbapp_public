//
// Created by 李卫东 on 2019-02-19.
//
#pragma once

#include <hb/log_plugin/log_plugin.h>
#include <hb/send_mail_plugin/send_mail_plugin_impl.h>
// #include <hb/thread_pool_plugin/thread_pool_plugin.h>

#include <appbase/application.hpp>
#include <functional>
#include <string>

using namespace std;

namespace hb::plugin {
    using namespace appbase;
    
    class send_mail_plugin : public appbase::plugin<send_mail_plugin> {
      public:
        APPBASE_PLUGIN_REQUIRES((log_plugin))
        send_mail_plugin();
        virtual ~send_mail_plugin();
        virtual void set_program_options(options_description &, options_description &) override;
        void plugin_initialize(const variables_map &);
        void plugin_startup();
        void plugin_shutdown();

        void send_mail(const string &to, const string &subject, const string &content) {
            my->send_mail(to, subject, content);
        }
      private:
        shared_ptr<send_mail_plugin_impl> my;
    };
}  // namespace hb
