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
        // target: a/b/c  拆分成"a","a-b","a-b-c"一共3个target, num是target的数量
        for (int num=1; num<=target_split.size(); num++) {
            string t;
            for (int i=0; i<num; i++) {
                t += target_split[i]+"-";
            }
            boost::trim_if(t,boost::is_any_of("-\t ")); 
            targets.push_back(boost::to_lower_copy(t));
        }
        return targets;
    }
    template<class REQ_TYPE>
    signal_results handle::deal_request(const REQ_TYPE &&req){
        auto targets = split_target(req.target);
        signal_results results;
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
                auto one_result = sig(req.body());
                results.insert(results.end(),one_result.begin(),one_result.end());
            }
        }
        return results;
    }
    
    template<class REQ_TYPE, class Send>
    void handle::request(const REQ_TYPE&& req, Send&& send) {
        ptree pt_res;
        pt_res.put("error","ok");
        pt_res.put("type","string-array");
        pt_res.put("target",req.target);
        ptree results;
        
        auto const send_response = [&](unsigned status){
            pt_res.put_child("results",results);
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
            auto deal_results = deal_request(req);
            if(deal_results.size()==0){
                pt_res.put("error","Target has no corresponding processing method!");
                send_response(http::status::bad_request);
                return;
            }
            for(auto &item : deal_results){
                results.push_back(std::make_pair("",item));
            }
            send_response(http::status::ok);
        hb_catch([&](const auto &e){
            pt_res.put("error",log_throw("do handle_request work error", e));
            send_response(http::status::internal_server_error);
        })
    }
}