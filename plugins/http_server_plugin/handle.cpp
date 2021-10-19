#include <hb/http_server_plugin/handle.h>
#include <boost/algorithm/string.hpp>


namespace hb::http_server { 

     map<string, shared_ptr<signal_type>> registed_signals_;
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
        string trim_target = boost::trim_copy_if(target,boost::is_any_of("/_-\t "));
        vector<string> target_split;
        boost::split(target_split, trim_target, boost::is_any_of("/_-"));

        vector<string> targets;
        // target: a/b/c  拆分成"a","a/b","a/b/c"一共3个target, num是target的数量
        for (int num=1; num<=target_split.size(); num++) {
            string t;
            for (int i=0; i<num; i++) {
                t += target_split[i]+"/";
            }
            boost::trim_if(t,boost::is_any_of("/\t ")); 
            targets.push_back(boost::to_lower_copy(t));
        }
        return targets;
    }
    template<class REQ_TYPE>
    ptree handle::deal_request(const REQ_TYPE &&req){
        auto targets = split_target(req.target);
        ptree tmp_result;
        stringstream stream(req.body());
        read_json(stream,tmp_result);

        for(auto &target : targets) {
            shared_ptr<signal_type> sig;
            {
                std::unique_lock<std::mutex> lock(signals_mutex_);
                decltype(registed_signals_.end()) it = registed_signals_.find(target);
                if(it != registed_signals_.end()){
                    sig = it->second;
                }
            }
            if (sig && sig->num_slots()>0) {
                // tmp_result一开始是req.body, 每一层处理自行修改内容当作下一层处理参数
                // a(tmp_result)的结果当成（a/b)的参数 （a/b)的结果当成(a/b/c)的参数
                tmp_result = *(*sig)(tmp_result);   //sig() return a boost::optional containing the result returned by the last slot called.
            }
        }
        return tmp_result;
    }
    
    template<class REQ_TYPE, class Send>
    void handle::request(const REQ_TYPE&& req, Send&& send) {
        auto const send_response = [&](unsigned status, const ptree &pt_res){
            stringstream stream;
            write_json(stream,pt_res);
            http::response<http::string_body> res{status, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/text");
            res.keep_alive(req.keep_alive());
            res.body() = std::string(stream.str());
            res.prepare_payload();
            send(std::move(res));
        };
        hb_try
            auto result = deal_request(req);
            if(result.empty()){
                ptree res;
                res.put("error","Target has no corresponding processing method!");
                res.put("target",req.target);
                send_response(http::status::bad_request, res);
                return;
            }
            send_response(http::status::ok, result);
        hb_catch([&](const auto &e){
            ptree res;
            res.put("error",log_throw("do handle_request work error", e));
            res.put("target",req.target);
            send_response(http::status::internal_server_error, res);
        })
    }
}