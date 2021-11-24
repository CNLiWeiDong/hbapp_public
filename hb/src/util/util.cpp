#include <hb/util/util.h>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>

namespace hb::util {  

    vector<string> split(const string &str, const string &chars) {
        vector<string> arr;
        boost::split(arr, str, boost::is_any_of(chars.c_str()));
    }
} // namespace hb
