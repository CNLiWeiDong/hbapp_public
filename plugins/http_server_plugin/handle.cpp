#include <hb/http_server_plugin/handle.h>
#include <boost/algorithm/string.hpp>


namespace hb::http_server { 

    map<string, shared_ptr<signal_type>> handle::registed_signals_;
    mutex handle::signals_mutex_;

    shared_ptr<signal_type> handle::regist(const string &target){
        std::unique_lock<std::mutex> lock(signals_mutex_); 
        auto it = registed_signals_.find(target);
        if(it!=registed_signals_.end())
            return it->second;
        shared_ptr<signal_type> sig = make_shared<signal_type>();
        registed_signals_.insert(decltype(registed_signals_)::value_type(target,std::move(sig)));
        return registed_signals_[target];
    }
    void handle::connect(const string &target, deal_fun fun){
        auto sig = regist(target);
        sig->connect(fun);
    }
    vector<string> handle::split_target(const string& target) {
        string trim_target = boost::trim_copy_if(target,boost::is_any_of("/\t ")); // "/a/b/c 变成a/b/c"
        vector<string> target_split;
        boost::split(target_split, trim_target, boost::is_any_of("/")); // a,b,c

        vector<string> targets;
        // target: a/b/c  拆分成"a","a/b","a/b/c"一共3个target, num是target的数量
        for (int num=1; num<=target_split.size(); num++) {
            string t;
            for (int i=0; i<num; i++) {
                t += "/"+target_split[i]; // /a /a/b /a/b/c
            }
            targets.push_back(boost::to_lower_copy(t));
        }
        return targets;
    }

    void handle::deal_request(deal_request_data &data){
            string req_target = data.req.get<string>("target");
            auto targets = split_target(req_target);
            for(auto &target : targets) {
                if (data.status!=deal_status::ok) {
                    break;
                }
                shared_ptr<signal_type> sig;
                {
                    std::unique_lock<std::mutex> lock(signals_mutex_);
                    decltype(registed_signals_.end()) it = registed_signals_.find(target);
                    if(it != registed_signals_.end()){
                        sig = it->second;
                    }
                }
                if (sig && sig->num_slots()>0) {
                    // data.result一开始是空, 每一层处理自行添加内容到result当作下一层处理参数
                    // a(data)的结果当成（a/b)的参数 （a/b)的结果当成(a/b/c)的参数
                    (*sig)(data);   //sig() return a boost::optional containing the result returned by the last slot called.
                    data.deal_num++;
                    data.deal_targets.push_back(target);
                }
            }
        }
}