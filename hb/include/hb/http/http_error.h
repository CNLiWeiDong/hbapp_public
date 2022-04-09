#pragma once

#include <hb/error/exception.h>

namespace hb {
    namespace http {
        using namespace hb::error;

        class hb_http_exception : public hb_exception<hb_http_exception, SN("http")> {
          public:
            hb_http_exception() : hb_exception<hb_http_exception, SN("http")>() {}
        };
    }  // namespace http
}  // namespace hb