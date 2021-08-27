#pragma once

#include <hb/error/exception.h>

namespace hb::plugin {
    using namespace hb::error;

    class http_server_exception : public hb_exception<http_server_exception, SN("httpserver")> {
    public:
        http_server_exception():hb_exception<http_server_exception, SN("httpserver")>() {

        }
    };
}