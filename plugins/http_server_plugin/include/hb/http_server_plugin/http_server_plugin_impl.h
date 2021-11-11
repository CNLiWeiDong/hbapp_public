//
// Created by 李卫东 on 2019-02-18.
//

#ifndef SIGN_HTTP_SERVER_PLUGIN_IMPL_H
#define SIGN_HTTP_SERVER_PLUGIN_IMPL_H

#include "boost/asio.hpp"
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <functional>
#include <string>
#include <hb/log/log.h>
#include <hb/http_server_plugin/listener.h>
#include <hb/http_server_plugin/handle.h>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace hb::http_server;
    
    struct http_options {
        bool https_enable = false;
        string host;
        int port = 0;
        uint64_t body_size = 0;
        uint32_t expires_seonds = 0;
        string dhparam_file;
        string certificate_file;
        string private_file;
        string private_password;
        // vector<string> enable_paths;
    };

    class http_server_plugin_impl: public std::enable_shared_from_this<http_server_plugin_impl>{
    public:
        http_server_plugin_impl(const http_options &opt):
            http_options_(opt)
        {};
        ~http_server_plugin_impl();
        void start();
    private:
        std::shared_ptr<ssl::context> load_ssl_cert();
        void start_http_server(std::shared_ptr<net::io_context> ioc);
        void start_https_server(std::shared_ptr<net::io_context> ioc);
    private:
        http_options http_options_;
    };
} }

#endif //SIGN_HTTP_SERVER_PLUGIN_IMPL_H
