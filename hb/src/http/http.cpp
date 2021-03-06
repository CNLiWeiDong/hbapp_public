#include <hb/error/exception.h>
#include <hb/http/http.h>
#include <hb/sync/sync_signal.h>
// #include <thread>
#include <boost/thread.hpp>

namespace hb {
    namespace http {

        // **同步asio设置超时没有效果，为了设置超时，使用异步+sync实现同步功能**
        int http::request() {
            hb_try {
                // The io_context is required for all I/O
                net::io_context ioc;
                // These objects perform our I/O
                tcp::resolver resolver(ioc);
                beast::tcp_stream stream(ioc);
                // 同步过期无作用
                stream.expires_after(std::chrono::seconds(expires_));
                // Look up the domain name
                auto const results = resolver.resolve(host_, port_);
                // Make the connection on the IP address we get from a lookup
                stream.connect(results);
                // Send the HTTP request to the remote host
                beast::http::write(stream, req_);
                // This buffer is used for reading and must be persisted
                beast::flat_buffer buffer;
                // Declare a container to hold the response
                beast::http::response<beast::http::string_body> res;
                // beast::http::response<beast::http::dynamic_body> res;
                // Receive the HTTP response
                beast::http::read(stream, buffer, res);
                // Write the message to standard out
                res_body_ = res.body();  // boost::beast::buffers_to_string(res.body().data()); //
                                         // boost::asio::buffer_cast<const char*>(res.body.data());
                // Gracefully close the socket
                beast::error_code ec;
                stream.socket().shutdown(tcp::socket::shutdown_both, ec);
                if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};
                return 200;
            }
            hb_catch([&](const auto &e) {
                res_body_ = log_throw("http request error.", e);
                return 500;
            });
            return 0;
        }
        
        int http::request(net::io_context &ioc) {
            auto signal = make_shared<hb::sycn::sync_signal>(true, expires_+1);
            shared_ptr<int> st = make_shared<int>(400);
            shared_ptr<string> res = make_shared<string>("time out");

            auto sess = std::make_shared<session>(ioc, req_, 
                [=](const int &status, const std::string &response){
                    hb_try{
                        *st = status;
                        *res = response;
                        signal->send();
                    }
                    hb_catch([&](const auto &e) {
                        log_throw("sync post http error:", e);
                    });
                    
                });
            sess->expires(expires_);
            sess->request(host_, port_);
            signal->wait();
            res_body_ = *res;
            return *st;
        }
        int http::get() {
            req_.method(beast::http::verb::get);
            return request();
        }
        int http::get(net::io_context &ioc) {
            req_.method(beast::http::verb::get);
            return request(ioc);
        }
        int http::post() {
            req_.method(
                beast::http::verb::post);  // http::request<http::string_body> req{http::verb::post,
                                           // target, 10}; target() version()
            req_.set(beast::http::field::content_length,
                     boost::lexical_cast<std::string>(req_body_.size()));
            // req_.set(beast::http::field::body, req_body_);
            req_.body() = req_body_;  // beast::http::field::body不起作用
            req_.prepare_payload();
            return request();
        }
        int http::post(net::io_context &ioc) {
            req_.method(
                beast::http::verb::post);  // http::request<http::string_body> req{http::verb::post,
                                           // target, 10}; target() version()
            req_.set(beast::http::field::content_length,
                     boost::lexical_cast<std::string>(req_body_.size()));
            // req_.set(beast::http::field::body, req_body_);
            req_.body() = req_body_;  // beast::http::field::body不起作用
            req_.prepare_payload();
            return request(ioc);
        }
        void http::post(net::io_context &ioc, const response_fun_type &callback) {
            req_.method(beast::http::verb::post);
            req_.set(beast::http::field::content_length,
                     boost::lexical_cast<std::string>(req_body_.size()));
            // req_.set(beast::http::field::body, req_body_);
            req_.body() = req_body_;
            req_.prepare_payload();

            auto sess = std::make_shared<session>(ioc, req_, callback);
            sess->request(host_, port_);
        }
        void http::get(net::io_context &ioc, const response_fun_type &callback) {
            req_.method(beast::http::verb::get);
            auto sess = std::make_shared<session>(ioc, req_, callback);
            sess->request(host_, port_);
        }
    }  // namespace http
}  // namespace hb