//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/json_table_plugin/json_table_plugin_impl.h>

namespace hb::plugin {
    json_table_plugin_impl::json_table_plugin_impl() {}
    json_table_plugin_impl::~json_table_plugin_impl() {}
    shared_ptr<const ptree> json_table_plugin_impl::get_item_byid(const string &table_name,
                                                                  uint32_t id) {
        constant_table::index<_id>::type &find_id = _json_tables[table_name].get<_id>();
        constant_table::index<_id>::type::iterator iter = find_id.find(id);
        if (iter != find_id.end()) {
            return iter->json;
        }
        hb_throw(
            json_table_exception().msg("read json error file:%s id:%d", table_name.c_str(), id));
    }
    constant_table::index<_id>::type &json_table_plugin_impl::get_id_items(
        const string &table_name) {
        return _json_tables[table_name].get<_id>();
    }
    shared_ptr<const ptree> json_table_plugin_impl::get_item_bykey(const string &table_name,
                                                                   const string &key) {
        constant_table::index<_key>::type &find_key = _json_tables[table_name].get<_key>();
        constant_table::index<_key>::type::iterator iter = find_key.find(key);
        if (iter != find_key.end()) {
            return iter->json;
        }
        hb_throw(json_table_exception().msg("read json error file:%s key:%s", table_name.c_str(),
                                            key.c_str()));
    }
    constant_table::index<_key>::type &json_table_plugin_impl::get_key_items(
        const string &table_name) {
        return _json_tables[table_name].get<_key>();
    }
    void json_table_plugin_impl::load_json(const bfs::path &src_path) {
        bfs::directory_iterator end;
        for (bfs::directory_iterator dir(src_path); dir != end; dir++) {
            if (dir->path().extension() == ".json") {
                log_debug << "json_table_plugin load_json json file:"
                          << dir->path().filename().string();
                string basename = dir->path().stem().string();
                boost::to_lower(basename);
                auto it = _json_tables.find(basename);
                if (it == _json_tables.end()) {
                    constant_table table;
                    _json_tables[basename] = table;
                }

                ptree json_data;
                read_json(dir->path().string(), json_data);
                if (json_data.empty()) {
                    hb_throw(json_table_exception().msg("read json file is empty! %s",
                                                        dir->path().string()));
                }
                for (auto &p : json_data) {
                    constant item;
                    item.id = p.second.get("id", 0);
                    if (item.id == 0) {
                        hb_throw(json_table_exception().msg("read json file no id exist! %s",
                                                            dir->path().string()));
                    }
                    string key = p.second.get("key", "");
                    key == "" ? item.key = to_string(item.id) : item.key = key;
                    item.json = make_shared<const ptree>(std::move(p.second));
                    _json_tables[basename].insert(std::move(item));
                }
            }
            if (bfs::is_directory(*dir)) {
                load_json(*dir);
            }
        }
    }
    void json_table_plugin_impl::start() {}

}  // namespace hb::plugin

// std::cout << dir << std::endl;
// std::cout << dir.string() << std::endl; //转换成std::string 类型
// std::cout << dir.root_name()<< std::endl; //盘符名：C:
// std::cout << dir.root_directory()<< std::endl; //根目录：”\”
// std::cout << dir.root_path()<< std::endl; //根路径：”C:\”
// std::cout << dir.relative_path()<< std::endl; //
// 相对路径:Windows\System32\services.exe std::cout << dir.parent_path()<<
// std::endl; //上级目录：C:\Windows\System32 std::cout << dir.filename()<<
// std::endl; //文件名：services.exe std::cout << dir.stem()<< std::endl;
// //不带扩展的文件名：services std::cout << dir.extension()<< std::endl;
// //扩展名：.exe std::cout << dir.replace_extension(“.txt”)//替换扩展名为txt