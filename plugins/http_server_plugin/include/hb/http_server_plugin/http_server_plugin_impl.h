//
// Created by 李卫东 on 2019-02-18.
//

#ifndef SIGN_HTTP_SERVER_PLUGIN_IMPL_H
#define SIGN_HTTP_SERVER_PLUGIN_IMPL_H

#include <hb/http_server_plugin/handler.h>
#include <hb/http_server_plugin/listener.h>
#include <hb/log/log.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "boost/asio.hpp"

namespace hb {
    namespace plugin {
        using namespace std;
        using namespace hb::http_server;

        struct http_options {
            bool https_enable = false;
            string host = {"127.0.0.1"};
            int port = 8080;
            uint64_t body_size = 1024 * 1024;
            uint32_t expires_seonds = 30;
            string dhparam_file;
            string certificate_file;
            string private_file;
            string private_password;
        };

        class http_server_plugin_impl
            : public std::enable_shared_from_this<http_server_plugin_impl> {
          public:
            http_server_plugin_impl(const http_options &opt) : http_options_(opt){};
            ~http_server_plugin_impl();
            void start();
            std::shared_ptr<handler> start_default_server();
            std::shared_ptr<handler> new_server(const http_options &options);
            std::shared_ptr<handler> default_handle() { return handle_; };

          private:
            std::shared_ptr<ssl::context> load_ssl_cert(const http_options &options);
            std::shared_ptr<handler> start_http_server(std::shared_ptr<net::io_context> ioc,
                                                      const http_options &options);
            std::shared_ptr<handler> start_https_server(std::shared_ptr<net::io_context> ioc,
                                                       const http_options &options);

          private:
            http_options http_options_;
            std::shared_ptr<handler> handle_;
        };
    }  // namespace plugin
}  // namespace hb

#endif  // SIGN_HTTP_SERVER_PLUGIN_IMPL_H
