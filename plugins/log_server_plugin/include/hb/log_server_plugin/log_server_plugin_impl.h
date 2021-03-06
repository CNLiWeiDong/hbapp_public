//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <hb/log/log.h>

#include <appbase/application.hpp>
#include <atomic>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace hb {
    namespace plugin {
        using namespace std;
        using namespace appbase;
        using boost::asio::ip::udp;

        class log_server_plugin_impl : public std::enable_shared_from_this<log_server_plugin_impl> {
          public:
            void start();

          public:
            log_server_plugin_impl(const int &port);
            ~log_server_plugin_impl();
            void msg_after(const std::string &val) { msg_after_ = val; }
            void do_receive();

          private:
            shared_ptr<udp::socket> socket_;
            udp::endpoint sender_endpoint_;
            enum { max_length = 2048 };
            char read_data_[max_length];
            std::string msg_after_;
            // char* read_data_ = (char*)malloc(data_len+1);
            // std::shared_ptr<char> buf_ptr(buf,free_char_data);
        };
    }  // namespace plugin
}  // namespace hb
