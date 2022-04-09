#pragma once

#include <hb/error/exception.h>

namespace hb {
    namespace plugin {
        using namespace hb::error;

        class mysql_exception : public hb_exception<mysql_exception, SN("mysql.p")> {
          public:
            mysql_exception() : hb_exception<mysql_exception, SN("mysql.p")>() {}
        };
    }  // namespace plugin
}  // namespace hb