#include <hb/error/exception.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>

namespace hb {
    namespace error {
        std::string exception_to_json(const hb_exception_base &e) {
            boost::property_tree::ptree pt_res;
            pt_res.put("code", *boost::get_error_info<err_code>(e));
            pt_res.put("msg", *boost::get_error_info<err_msg>(e));
            pt_res.put("type", *boost::get_error_info<err_type>(e));
            pt_res.put("file", *boost::get_error_info<err_file>(e));
            pt_res.put("function", *boost::get_error_info<err_fun>(e));
            pt_res.put("line", *boost::get_error_info<err_line>(e));
            std::stringstream stream;
            boost::property_tree::write_json(stream, pt_res);
            return stream.str();
        }

    }  // namespace error
}  // namespace hb