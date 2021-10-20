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

    // typedef vector<string> signal_results;
    typedef function<ptree(const ptree &)> deal_fun;
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
    typedef boost::signals2::signal<ptree(const ptree &)> signal_type;

    class handle {
        public:
        static map<string, shared_ptr<signal_type>> registed_signals_; 
        static mutex signals_mutex_;

        static shared_ptr<signal_type> regist(const string &target);
        static void connect(const string &target, deal_fun fun);
        static vector<string> split_target(const string& target);
        static ptree deal_request(const string &req_target, const string &req_body);

        template<class REQ_TYPE, class Send>
        static void request(const REQ_TYPE&& req, Send&& send) {
            auto const send_response = [&](http::status status, const ptree &pt_res){
                stringstream stream;
                write_json(stream,pt_res);
                http::response<http::string_body> res{status, req.version()}; //
                // http::response<http::string_body> res{http::status::bad_request, req.version()};
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "application/text");
                res.keep_alive(req.keep_alive());
                res.body() = stream.str();
                res.prepare_payload();
                send(std::move(res));
            };
            std::string req_target(req.target());
            std::string req_body(req.body());
            hb_try
                auto result = deal_request(req_target, req_body);
                if(result.empty()){
                    ptree res;
                    res.put("error","Target has no corresponding processing method!");
                    res.put("target",req_target);
                    send_response(http::status::bad_request, res);
                    return;
                }
                result.put("target",req_target);
                send_response(http::status::ok, result);
            hb_catch([&](const auto &e){
                ptree res;
                res.put("error",log_throw("do handle_request work error", e));
                res.put("target",req_target);
                send_response(http::status::internal_server_error, res);
            })
        }
    };
}