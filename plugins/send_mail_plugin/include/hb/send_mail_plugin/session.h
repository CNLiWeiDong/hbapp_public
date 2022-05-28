#pragma once

#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

namespace hb:: plugin {
    using namespace std;
    using namespace boost::asio;
    using boost::asio::ip::tcp;

    class session {
      public:
        ~session(){};
        void host(const string& host) { _host = host; }
        void port(const string& port) { _port = port; }
        void user(const string& user) { _user = user; }
        void pass(const string& pass) { _pass = pass; }
        void to(const string& to) { _to = to; }
        void subject(const string& subject) { _subject = subject; }
        void content(const string& content) { _content = content; }
        void send(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& sock,
                  const string& msg) {
            boost::asio::write(sock, boost::asio::buffer(msg));
        }
        const string get_response(
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& sock) {
            boost::asio::streambuf buffer;
            string msg;
            boost::asio::read_until(sock, buffer, "\r\n");
            std::istream in(&buffer);
            std::getline(in, msg);
            return msg;
        }
        void mail();

      private:
        string _host = "";
        string _port = "25";
        string _user = "";
        string _pass = "";
        string _to = "";
        string _subject = "";
        string _content = "";
    };
};     // namespace hb