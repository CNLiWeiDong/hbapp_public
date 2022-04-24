//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <hb/json_table_plugin/json_table_error.h>
#include <hb/json_table_plugin/constant_var.h>
#include <hb/log/log.h>
#include <appbase/application.hpp>
#include <atomic>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/signals2/signal.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace hb {
    namespace plugin {
        using namespace std;
        using namespace appbase;
        using namespace boost::property_tree;
        namespace bfs = boost::filesystem;

        struct constant {
            uint32_t id;
            string key;
            shared_ptr<const ptree> json;
        };
        struct _id {};
        struct _key {};

        using constant_table = boost::multi_index::multi_index_container<
            constant,
            boost::multi_index::indexed_by<
                boost::multi_index::ordered_unique<
                    boost::multi_index::tag<_id>,
                    BOOST_MULTI_INDEX_MEMBER(constant, uint32_t, id)>,  // id为唯一索引，类似主键
                boost::multi_index::ordered_unique<
                    boost::multi_index::tag<_key>,
                    BOOST_MULTI_INDEX_MEMBER(constant, string, key)>  // key为唯一索引，类似主键
                >>;

        class json_table_plugin_impl : public std::enable_shared_from_this<json_table_plugin_impl> {
          public:
            void start();

          public:
            json_table_plugin_impl();
            ~json_table_plugin_impl();
            shared_ptr<const ptree> get_item_byid(const string &table_name, uint32_t id);
            constant_table::index<_id>::type &get_id_items(const string &table_name);
            shared_ptr<const ptree> get_item_bykey(const string &table_name, const string &key);
            constant_table::index<_key>::type &get_key_items(const string &table_name);
            void load_json(const bfs::path &src_path);

          private:
            map<string, constant_table> _json_tables;
        };
    }  // namespace plugin
}  // namespace hb
