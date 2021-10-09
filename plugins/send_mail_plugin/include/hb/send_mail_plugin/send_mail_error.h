#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

<<<<<<< HEAD
    class send_mail_exception : public hb_exception<send_mail_exception> {
    public:
        send_mail_exception():hb_exception<send_mail_exception>(send_mail_plugin_no, get_title(send_mail_plugin_no)) {
=======
    class send_mail_exception : public hb_exception<send_mail_exception, SN("sendmail")> {
    public:
        send_mail_exception():hb_exception<send_mail_exception, SN("sendmail")>() {
>>>>>>> 52017c3263517f7fdc73b63c5a3bdff1fc448352

        }
    };
} }