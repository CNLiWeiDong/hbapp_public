//
// Created by 李卫东 on 2019-02-19.
//
#pragma once

#include <hb/json_table_plugin/json_table_plugin_impl.h>
#include <hb/log_plugin/log_plugin.h>

#include <appbase/application.hpp>
#include <functional>
#include <string>

using namespace std;

namespace hb {
    namespace plugin {
        using namespace appbase;

        class json_table_plugin : public appbase::plugin<json_table_plugin> {
          public:
            APPBASE_PLUGIN_REQUIRES((log_plugin))
            json_table_plugin();
            virtual ~json_table_plugin();
            virtual void set_program_options(options_description &, options_description &) override;
            void plugin_initialize(const variables_map &);
            void plugin_startup();
            void plugin_shutdown();

            shared_ptr<const ptree> get_item_byid(const string &table_name, uint32_t id) {
                return my->get_item_byid(table_name, id);
            }
            constant_table::index<_id>::type &get_id_items(const string &table_name) {
                return my->get_id_items(table_name);
            }
            shared_ptr<const ptree> get_item_bykey(const string &table_name, const string &key) {
                return my->get_item_bykey(table_name, key);
            }
            constant_table::index<_key>::type &get_key_items(const string &table_name) {
                return my->get_key_items(table_name);
            }

          private:
            shared_ptr<json_table_plugin_impl> my;
        };
    }  // namespace plugin
}  // namespace hb
