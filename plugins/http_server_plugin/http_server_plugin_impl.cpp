//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/http_server_plugin/http_server_plugin_impl.h>
#include <hb/thread_pool_plugin/thread_pool_plugin.h>
#include <boost/algorithm/string.hpp>

namespace hb{ namespace plugin {
        http_server_plugin_impl::~http_server_plugin_impl(){
        }
        void http_server_plugin_impl::start_http_server(std::shared_ptr<net::io_context> ioc){
            auto const address = net::ip::make_address(http_options_.host);
            auto const port = static_cast<unsigned short>(http_options_.port);
            auto listien = std::make_shared<listener>(
                ioc,
                tcp::endpoint{address, port});
            listien->set_body_limit(http_options_.body_size);
            listien->set_expires_seconds(http_options_.expires_seonds);
            listien->run();
            log_info<<"http server ip: "<<http_options_.host<<" port: "<<http_options_.port<<" starting!";
        }
        void http_server_plugin_impl::start_https_server(std::shared_ptr<net::io_context> ioc){
            
        }
        void http_server_plugin_impl::start(){
            auto & thread_plugin = app().get_plugin<thread_pool_plugin>();
            auto abi = thread_plugin.get_api();
            auto ioc = abi->get_io_context();
            if(http_options_.https_enable){
                start_https_server(ioc);
            }else
            {
                start_http_server(ioc);
            }
        }
} }