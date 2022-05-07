#include <hb/json_table_plugin/json_table_plugin.h>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <vector>

namespace hb {
    namespace plugin {
        static appbase::abstract_plugin &_json_table_plugin
            = app().register_plugin<json_table_plugin>();
        json_table_plugin::json_table_plugin() {}
        json_table_plugin::~json_table_plugin() {}
        void json_table_plugin::set_program_options(options_description &cli,
                                                    options_description &cfg) {
            cfg.add_options()("constant-json-config-dir",
                              boost::program_options::value<string>()->default_value("json"),
                              "constant-json-config-dir to load all json file!");
        }
        void json_table_plugin::plugin_initialize(const variables_map &options) {
            log_info("json_table_plugin::plugin_initialize");
            hb_try {
                my = make_shared<json_table_plugin_impl>();

                auto workaround = options["constant-json-config-dir"].as<std::string>();
                bfs::path json_path = workaround;
                if (json_path.is_relative()) {
                    json_path = app().config_dir() / json_path;
                }
                log_debug("json_table_plugin::plugin_initialize json path:%s",json_path.string().c_str());
                my->load_json(json_path);
            }
            hb_catch([&](const auto &e) {
                log_throw("block_control_plugin::plugin_initialize", e);
                std::exit(-2);
            });
        }
        void json_table_plugin::plugin_startup() {
            log_info("json_table_plugin::plugin_startup");
            my->start();
        }
        void json_table_plugin::plugin_shutdown() {
            log_info("json_table_plugin::plugin_shutdown");
            if (my) my.reset();
        }
    }  // namespace plugin
}  // namespace hb