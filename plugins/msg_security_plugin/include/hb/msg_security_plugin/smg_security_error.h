#pragma once

#include <hb/error/exception.h>

namespace hb::plugin {
    using namespace hb::error;

    class smg_security_error : public hb_exception<smg_security_error, SN("msgsecurity")> {
      public:
        smg_security_error() : hb_exception<smg_security_error, SN("msgsecurity")>() {}
    };
}  // namespace hb::plugin