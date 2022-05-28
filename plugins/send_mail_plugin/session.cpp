#include <hb/crypto/base.h>
#include <hb/log/log.h>
#include <hb/send_mail_plugin/send_mail_error.h>
#include <hb/send_mail_plugin/session.h>

namespace hb {
    namespace plugin {
        void session::mail() {
            log_info("session::mail begin!!!");
            if (_host.size() == 0) {
                hb::plugin::send_mail_exception e;
                e.msg("Error! Need set host!");
                hb_throw(e);
            }
            if (_to.size() == 0) {
                hb::plugin::send_mail_exception e;
                e.msg("Error! Need set mail to mail!");
                hb_throw(e);
            }
            boost::asio::io_service ioc;
            tcp::resolver resolver(ioc);
            tcp::resolver::query query(_host, _port);
            tcp::resolver::iterator it_endpt = resolver.resolve(query);
            tcp::resolver::iterator end;
            if (it_endpt == end) {
                hb::plugin::send_mail_exception e;
                e.msg("Error! resolve host error!");
                hb_throw(e);
            }
            // tcp::socket sock(ioc);
            boost::asio::ssl::context ctx(ssl::context::tlsv12_client);
            ctx.set_verify_mode(ssl::verify_none);
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket> sock(ioc, ctx);
            boost::system::error_code error;
            // sock.connect(*it_endpt++,error);
            sock.lowest_layer().connect(*it_endpt++, error);
            sock.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
            if (error) {
                // sock.close();
                sock.shutdown(error);
                hb::plugin::send_mail_exception e;
                e.msg("HOST can't be connected!");
                hb_throw(e);
            }
            sock.handshake(boost::asio::ssl::stream_base::client);
            send(sock, "HELO " + _user + "\r\n");
            log_info("mail response:%s", get_response(sock).c_str());

            send(sock, "auth login\r\n");
            log_info("mail response:%s", get_response(sock).c_str());
            send(sock, hb::crypto::base64(_user) + "\r\n");
            log_info("mail response:%s", get_response(sock).c_str());
            send(sock, hb::crypto::base64(_pass) + "\r\n");
            log_info("mail response:%s", get_response(sock).c_str());

            send(sock, "mail from: <" + _user + ">\r\n");
            log_info("mail response:%s", get_response(sock).c_str());
            send(sock, "rcpt to: <" + _to + ">\r\n");
            log_info("mail response:%s", get_response(sock).c_str());
            send(sock, "data\r\n");
            log_info("mail response:%s", get_response(sock).c_str());
            send(sock, "From:" + _user + "\r\n");
            // log_info("mail response:%s", get_response(sock).c_str());
            send(sock,"subject:"+_subject+"\r\nMime-Version: 1.0\r\n"
                    "Content-Type: multipart/mixed;   "
                    "boundary=\"INVT\"\r\n\r\n");
            // log_info("mail response:%s", get_response(sock).c_str());
            send(sock,
                 "--INVT\r\nContent-Type: text/plain;\r\n  "
                 "charset=\"utf8\"\r\n\r\n"
                     + _content + "\r\n\r\n");
            // log_info("mail response:%s", get_response(sock).c_str());
            send(sock, "\r\n.\r\n");
            log_info("mail response:%s", get_response(sock).c_str());
            send(sock, "quit\r\n");
            log_info("mail response:%s", get_response(sock).c_str());
            sock.shutdown(error);
            log_info("session::mail end!!!");
        }
    }  // namespace plugin
};     // namespace hb