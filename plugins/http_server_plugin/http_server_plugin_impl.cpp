//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/http_server_plugin/http_server_plugin_impl.h>
#include <hb/thread_pool_plugin/thread_pool_plugin.h>

#include <boost/algorithm/string.hpp>

namespace hb {
    namespace plugin {
        http_server_plugin_impl::~http_server_plugin_impl() {}
        std::shared_ptr<ssl::context> http_server_plugin_impl::load_ssl_cert(
            const http_options &options) {
            auto self = shared_from_this();

            std::shared_ptr<ssl::context> ctx = make_shared<ssl::context>(ssl::context::sslv23);
            ctx->set_options(ssl::context::default_workarounds | ssl::context::no_sslv2
                             | ssl::context::no_sslv3 | ssl::context::no_tlsv1
                             | ssl::context::no_tlsv1_1 | ssl::context::single_dh_use);
            // 解决 handshake error: session id context uninitialized=
            // ctx->set_options(ssl::context::ssl_op_no_ticket);
            static int context_id = 1;
            if (!SSL_CTX_set_session_id_context(
                    ctx->native_handle(), (const unsigned char *)&context_id, sizeof(context_id))) {
                LOG_FATAL("SSL set session id context failed");
                std::exit(-2);
            }
            ctx->set_password_callback([=](std::size_t, ssl::context_base::password_purpose) {
                return options.private_password.c_str();
            });
            auto config_path = app().config_dir();
            ctx->use_certificate_chain_file((config_path / options.certificate_file).string());
            ctx->use_private_key_file((config_path / options.private_file).string(),
                                      ssl::context::pem);
            ctx->use_tmp_dh_file((config_path / options.dhparam_file).string());
            ctx->set_verify_mode(ssl::verify_peer);  // |ssl::verify_fail_if_no_peer_cert);
            return ctx;
        }
        std::shared_ptr<handler> http_server_plugin_impl::start_http_server(
            std::shared_ptr<net::io_context> ioc, const http_options &options) {
            log_info("http_server_plugin_impl::start_http_server");
            auto const address = net::ip::make_address(options.host);
            auto const port = static_cast<unsigned short>(options.port);
            auto l = std::make_shared<listener>(ioc, tcp::endpoint{address, port});
            l->set_body_limit(options.body_size);
            l->set_expires_seconds(options.expires_seonds);
            l->run();
            log_info("http server ip: %s port: %d starting!", options.host, options.port);
            return l->handle();
        }
        std::shared_ptr<handler> http_server_plugin_impl::start_https_server(
            std::shared_ptr<net::io_context> ioc, const http_options &options) {
            log_info("http_server_plugin_impl::start_https_server");
            auto const address = net::ip::make_address(options.host);
            auto const port = static_cast<unsigned short>(options.port);
            auto ctx = load_ssl_cert(options);
            auto l = std::make_shared<listener>(ioc, ctx, tcp::endpoint{address, port});
            l->set_body_limit(options.body_size);
            l->set_expires_seconds(options.expires_seonds);
            l->run();
            log_info("https server ip: %s port: %d starting!", options.host, options.port);
            return l->handle();
        }
        void http_server_plugin_impl::start() {}
        std::shared_ptr<handler> http_server_plugin_impl::start_default_server() {
            log_info("http_server_plugin_impl::start_default_server");
            auto &thread_plugin = app().get_plugin<thread_pool_plugin>();
            auto abi = thread_plugin.get_api();
            auto ioc = abi->get_io_context();
            std::shared_ptr<handler> h;
            if (http_options_.https_enable) {
                h = start_https_server(ioc, http_options_);
            } else {
                h = start_http_server(ioc, http_options_);
            }
            handle_ = h;
            return h;
        }

        std::shared_ptr<handler> http_server_plugin_impl::new_server(const http_options &options) {
            log_info("http_server_plugin_impl::new_server start");
            auto &thread_plugin = app().get_plugin<thread_pool_plugin>();
            auto abi = thread_plugin.get_api();
            auto ioc = abi->get_io_context();
            std::shared_ptr<handler> h;
            if (options.https_enable) {
                h = start_https_server(ioc, options);
            } else {
                h = start_http_server(ioc, options);
            }
            return h;
        }
    }  // namespace plugin
}  // namespace hb