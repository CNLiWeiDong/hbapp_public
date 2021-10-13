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

    typedef vector<string> signal_results;
    typedef function<string(const string &)> deal_fun;
    template <typename T> 
    struct all_element 
    { 
        typedef T result_type; 
        
        template <typename InputIterator> 
        T operator()(InputIterator first, InputIterator last) const 
        { 
            return T(first, last); 
        } 
    };
    typedef boost::signals2::signal<string(const string &), all_element<signal_results>> signal_type;

    class handle {
        public:
        static map<string, shared_ptr<signal_type>> registed_signals_; 
        static mutex signals_mutex_;

        static shared_ptr<signal_type> regist(const string &target);
        static void connect(const string &target, deal_fun fun);
        static vector<string> split_target(const string& target);
        template<class REQ_TYPE>
        static signal_results deal_request(const REQ_TYPE &&req);
        template<class REQ_TYPE, class Send>
        static void request(const REQ_TYPE&& req, Send&& send);

        static void load_server_certificate(boost::asio::ssl::context& ctx);
    };
}