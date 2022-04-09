//
// Created by 李卫东 on 2019-01-14.
#pragma once

#include <mysql++.h>

#include <boost/noncopyable.hpp>
#include <string>

namespace hb {
    namespace mysql_plugin {
        using namespace std;
        using namespace mysqlpp;

        class mysql_plugin_impl;
        class mysql_connection : public Connection, public boost::noncopyable {
          public:
            virtual bool connect(const char* db = 0, const char* server = 0, const char* user = 0,
                                 const char* password = 0, unsigned int port = 0) {
                db_ = db;
                server_ = server;
                port_ = port;
                return Connection::connect(db, server, user, password, port);
            }
            bool status() {
                if (connected() && errnum() <= 0) {
                    return true;
                }
                return false;
            }
            void stop() { stoped_ = true; }
            void open() { stoped_ = false; }
            bool is_stoped() { return stoped_; }
            mysql_connection() : Connection(true) {}
            friend class mysql_plugin_impl;

          public:
            inline const string& server() { return server_; }
            inline const int& port() { return port_; }
            inline const string& db() { return db_; }

          private:
            string server_;
            string db_;
            int port_;
            std::atomic_bool stoped_ = {false};
        };

    }  // namespace mysql_plugin
}  // namespace hb
