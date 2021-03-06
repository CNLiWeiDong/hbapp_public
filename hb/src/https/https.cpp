#include <hb/https/https.h>
#include <hb/https/https_error.h>
#include <hb/sync/sync_signal.h>
#include <thread>

namespace hb::https {
    void https::load_certificates() {
        if (cert_str_.empty() && ctx_ == nullptr) {
            ctx_ = make_shared<ssl::context>(ssl::context::tlsv12_client);
            ctx_->set_verify_mode(ssl::verify_none);
            return;
        }
        if (ctx_) return;

        boost::system::error_code ec;
        ctx_ = make_shared<ssl::context>(ssl::context::tlsv12_client);
        ctx_->add_certificate_authority(boost::asio::buffer(cert_str_.data(), cert_str_.size()),
                                        ec);
        // Verify the remote server's certificate
        ctx_->set_verify_mode(ssl::verify_peer);
        if (ec) {
            // throw boost::system::system_error{ec};
            hb_throw(hb_https_exception().msg("load_certificates error no: %d msg: %s", ec.value(),
                                              ec.message().c_str()));
        }
    }
    int https::request() {
        hb_try {
            // The io_context is required for all I/O
            net::io_context ioc;
            // boost::system::error_code ec;
           // load_certificates();
            // These objects perform our I/O
            tcp::resolver resolver(ioc);
            beast::ssl_stream<beast::tcp_stream> stream(ioc, *ctx_);
            beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(expires_));
            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name(stream.native_handle(), host_.c_str())) {
                beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                     net::error::get_ssl_category()};
                hb_throw(
                    hb_https_exception().msg("load_certificates beaset::error_code: %d message: %s",
                                             ec.value(), ec.message().c_str()));
                // throw beast::system_error{ec};
            }
            // Look up the domain name
            auto const results = resolver.resolve(host_, port_);
            // Make the connection on the IP address we get from a lookup
            beast::get_lowest_layer(stream).connect(results);
            // Perform the SSL handshake
            stream.handshake(ssl::stream_base::client);
            // Send the HTTP request to the remote host
            beast::http::write(stream, req_);
            // This buffer is used for reading and must be persisted
            beast::flat_buffer buffer;
            // Declare a container to hold the response
            beast::http::response<beast::http::string_body> res;
            // Receive the HTTP response
            beast::http::read(stream, buffer, res);
            // Write the message to standard out
            res_body_ = res.body();
            // Gracefully close the stream
            beast::error_code ec;
            beast::get_lowest_layer(stream).cancel();
            stream.shutdown(ec);
            if(ec && ec != net::error::eof && ec != beast::errc::not_connected && ec!= boost::asio::ssl::error::stream_truncated)
            {
                throw beast::system_error{ec};
            }
            return 200;
        }
        hb_catch([&](const auto &e) {
            res_body_ = log_throw("do https request error", e);
            return 500;
        });
        return 0;
    }
    int https::request(net::io_context &ioc) {
        auto signal = make_shared<hb::sycn::sync_signal>(true, expires_+1);
        shared_ptr<int> st = make_shared<int>(400);
        shared_ptr<string> res = make_shared<string>("time out");

        auto sess = std::make_shared<session>(ioc, *ctx_, req_, 
            [=](const int &status, const std::string &response){
                hb_try{
                    *st = status;
                    *res = response;
                    signal->send();
                }
                hb_catch([&](const auto &e) {
                    log_throw("sync post https error:", e);
                });
                
            });
        sess->expires(expires_);
        sess->request(host_, port_);
        signal->wait();
        res_body_ = *res;
        return *st;
    }
    
    int https::get() {
        load_certificates();
        req_.method(beast::http::verb::get);
        return request();
    }
    int https::get(net::io_context &ioc) {
        load_certificates();
        req_.method(beast::http::verb::get);
        return request(ioc);
    }
    int https::post() {
        load_certificates();
        req_.method(beast::http::verb::post);
        req_.set(beast::http::field::content_length,
                 boost::lexical_cast<std::string>(req_body_.size()));
        // req_.set(beast::http::field::body, req_body_);
        req_.body() = req_body_;  // beast::http::field::body????????????
        req_.prepare_payload();
        return request();
    }
    int https::post(net::io_context &ioc) {
        load_certificates();
        req_.method(beast::http::verb::post);
        req_.set(beast::http::field::content_length,
                 boost::lexical_cast<std::string>(req_body_.size()));
        // req_.set(beast::http::field::body, req_body_);
        req_.body() = req_body_;  // beast::http::field::body????????????
        req_.prepare_payload();
        return request(ioc);
    }
    void https::post(net::io_context &ioc, const response_fun_type &callback) {
        load_certificates();
        req_.method(beast::http::verb::post);
        req_.set(beast::http::field::content_length,
                 boost::lexical_cast<std::string>(req_body_.size()));
        // req_.set(beast::http::field::body, req_body_);
        req_.body() = req_body_;
        req_.prepare_payload();
        auto sess = std::make_shared<session>(ioc, *ctx_, req_, callback);
        sess->request(host_, port_);
    }
    void https::get(net::io_context &ioc, const response_fun_type &callback) {
        load_certificates();
        req_.method(beast::http::verb::get);
        auto sess = std::make_shared<session>(ioc, *ctx_, req_, callback);
        sess->request(host_, port_);
    }
}  // namespace hb::https