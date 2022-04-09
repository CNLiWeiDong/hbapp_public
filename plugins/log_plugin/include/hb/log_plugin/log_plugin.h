//
// Created by 李卫东 on 2019-11-11.
//

#ifndef CPP_SERVER_LOGGING_PLUGIN_H
#define CPP_SERVER_LOGGING_PLUGIN_H

#include <hb/log/log.h>

#include <appbase/application.hpp>
#include <functional>
#include <memory>
#include <string>
#include <thread>

using namespace std;

namespace hb {
    namespace plugin {
        using namespace appbase;
        class log_plugin : public appbase::plugin<log_plugin> {
          public:
            APPBASE_PLUGIN_REQUIRES()
            log_plugin();
            virtual ~log_plugin();
            virtual void set_program_options(options_description&, options_description&) override;
            void plugin_initialize(const variables_map&);
            void plugin_startup();
            void plugin_shutdown();
        };
    }  // namespace plugin
}  // namespace hb

#endif  // CPP_SERVER_LOGGING_PLUGIN_H
