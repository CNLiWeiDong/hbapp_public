#pragma once
#include <cstddef>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <hb/log_plugin/log_plugin.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/signals2/signal.hpp>

namespace hb::http_server {
    namespace beast = boost::beast;         // from <boost/beast.hpp>
    namespace http = beast::http;           // from <boost/beast/http.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    using namespace boost::property_tree;

    enum class deal_status: unsigned {
        ok = 0,
        error = 1,
        finished = 2,
        stop = 3
    };
    struct deal_request_data {
        ptree req;
        ptree result;
        unsigned deal_num = 0;
        vector<string> deal_targets;
        string deal_msg;
        deal_status status = deal_status::ok;
    };
    typedef function<void(deal_request_data &)> deal_fun;

    // template <typename T> 
    // struct all_element 
    // { 
    //     typedef T result_type; 
        
    //     template <typename InputIterator> 
    //     T operator()(InputIterator first, InputIterator last) const 
    //     { 
    //         return T(first, last); 
    //     } 
    // };
    // typedef boost::signals2::signal<string(const string &), all_element<signal_results>> signal_type;
    typedef boost::signals2::signal<void(deal_request_data &)> signal_type;

    class handle {
        public:
        static map<string, shared_ptr<signal_type>> registed_signals_; 
        static mutex signals_mutex_;

        static shared_ptr<signal_type> regist(const string &target);
        static void connect(const string &target, deal_fun fun);
        static vector<string> split_target(const string& target);
        static void deal_request(deal_request_data &data);

        template<class REQ_TYPE, class Send>
        static void request(const REQ_TYPE&& req, Send&& send) {
            
            std::string req_target(req.target());
            std::string req_body(req.body());
            deal_request_data data;
            stringstream stream(req_body);
            read_json(stream, data.req);
            data.req.put("target", req_target);
            data.result.put("target", req_target);
            data.result.put("error", ""); //默认没有错误 error不为空串时报错
            data.result.put("code",0); //默认没有错误 code 为0表示操作成功
            auto const send_response = [&](http::status status){
                stringstream stream;
                write_json(stream, data.result);
                http::response<http::string_body> res{status, req.version()}; 
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "application/text");
                res.keep_alive(req.keep_alive());
                res.body() = stream.str();
                res.prepare_payload();
                send(std::move(res));
            };
            hb_try
                deal_request(data);
                if(data.deal_num==0){
                    data.result.put("error","Target has no corresponding processing method!");
                    data.result.put("code",400);
                    send_response(http::status::bad_request);
                    return;
                }
                send_response(http::status::ok);
            hb_catch([&](const auto &e){
                data.result.put("error",log_throw("do handle_request work error", e));
                data.result.put("code",500);
                send_response(http::status::internal_server_error);
            })
        }
    };
}