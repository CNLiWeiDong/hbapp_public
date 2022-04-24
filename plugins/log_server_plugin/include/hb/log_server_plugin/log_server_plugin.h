//
// Created by 李卫东 on 2019-02-19.
//
#pragma once

#include <hb/log_plugin/log_plugin.h>
#include <hb/log_server_plugin/log_server_plugin_impl.h>

#include <appbase/application.hpp>
#include <functional>
#include <string>

using namespace std;

namespace hb {
    namespace plugin {
        using namespace appbase;

        class log_server_plugin : public appbase::plugin<log_server_plugin> {
          public:
            APPBASE_PLUGIN_REQUIRES((log_plugin))
            log_server_plugin();
            virtual ~log_server_plugin();
            virtual void set_program_options(options_description &, options_description &) override;
            void plugin_initialize(const variables_map &);
            void plugin_startup();
            void plugin_shutdown();

          private:
            shared_ptr<log_server_plugin_impl> my;
        };
    }  // namespace plugin
}  // namespace hb
