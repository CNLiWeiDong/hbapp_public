#pragma once

#include <hb/error/exception.h>

namespace hb {  namespace sycn {
    using namespace hb::error;

    class hb_sync_exception : public hb_exception<hb_sync_exception, SN("sycn")> {
        public:
        hb_sync_exception():hb_exception<hb_sync_exception, SN("sycn")>() {

        }
    };
} }