#pragma once
#include <hb/http/session.h>

namespace hb {
    namespace http {
        class http {
          public:
            http(const std::string &host, const std::string &port, const std::string target,
                 const int &expires = 30)
                : host_(host), port_(port), target_(target), expires_(expires) {
                req_.target(target);
                req_.version(11);
                req_.set(beast::http::field::host, host);
                req_.set(beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            }
            ~http() {}
            int post();
            int get();
            int post(net::io_context &ioc);
            int get(net::io_context &ioc);
            void post(net::io_context &ioc, const response_fun_type &callback);
            void get(net::io_context &ioc, const response_fun_type &callback);

          private:
            int request();
            int request(net::io_context &ioc);

          public:
            void host(const std::string &host) {
                host_ = host;
                req_.set(beast::http::field::host, host);
            }
            const std::string &host() const { return host_; }

            void version(const int &val) {
                version_ = val;
                req_.version(val);
            }
            const int &version() const { return version_; }

            void port(const std::string &port) { port_ = port; }
            const std::string &port() const { return port_; }

            const int &expires() const { return expires_; }
            void expires(const int &expires) { expires_ = expires; }

            void target(const std::string &target) {
                target_ = target;
                req_.target(target);
            }
            const std::string &target() const { return target_; }

            void req_body(const std::string &req_body) { req_body_ = req_body; }
            const std::string &req_body() const { return req_body_; }

            const std::string &res_body() const { return res_body_; }
            template <typename T>
            void set_req_param(const beast::http::field &field, const T &value) {
                req_.set(field, value);
            }

          private:
            std::string host_;
            std::string port_;
            std::string target_;
            std::string req_body_;
            int expires_;
            int version_;
            std::string res_body_;
            beast::http::request<beast::http::string_body> req_;
        };
    }  // namespace http
}  // namespace hb