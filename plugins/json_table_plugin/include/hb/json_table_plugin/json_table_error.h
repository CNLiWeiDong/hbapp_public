#pragma once

#include <hb/error/exception.h>

namespace hb::plugin {
    using namespace hb::error;

    class json_table_exception : public hb_exception<json_table_exception, SN("jsontable")> {
      public:
        json_table_exception() : hb_exception<json_table_exception, SN("jsontable")>() {}
    };
}  // namespace hb::plugin