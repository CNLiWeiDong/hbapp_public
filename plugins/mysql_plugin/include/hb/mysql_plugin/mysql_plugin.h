//
// Created by 李卫东 on 2019-02-26.
//
#pragma once

#include <hb/log_plugin/log_plugin.h>
#include <hb/mysql_plugin/mysql_plugin_impl.h>

#include <appbase/application.hpp>
#include <functional>
#include <memory>
#include <string>
#include <thread>

using namespace std;

namespace hb {
    namespace plugin {
        using namespace appbase;
        class mysql_plugin_api {
          public:
            mysql_plugin_api(shared_ptr<mysql_plugin_impl> _impl) : impl(_impl) {}

            shared_ptr<mysql_connection> connect(const char* db = 0, const char* ip = 0,
                                                 const char* user = 0, const char* password = 0,
                                                 unsigned int port = 0) {
                return impl->connect(db, ip, user, password, port);
            }
            void close(const shared_ptr<mysql_connection>& con) { impl->close(con); }

          private:
            shared_ptr<mysql_plugin_impl> impl;
        };
        class mysql_plugin : public appbase::plugin<mysql_plugin> {
          public:
            APPBASE_PLUGIN_REQUIRES((log_plugin))
            mysql_plugin();
            virtual ~mysql_plugin();
            virtual void set_program_options(options_description&, options_description&) override;

            void plugin_initialize(const variables_map&);
            void plugin_startup();
            void plugin_shutdown();
            shared_ptr<mysql_plugin_api> get_api() { return make_shared<mysql_plugin_api>(my); }

          private:
            shared_ptr<mysql_plugin_impl> my;
        };
    }  // namespace plugin
}  // namespace hb