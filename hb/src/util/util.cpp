#include <hb/util/util.h>

#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>

namespace hb::util {

    vector<string> split(const string &str, const string &chars) {
        vector<string> arr;
        boost::split(arr, str, boost::is_any_of(chars.c_str()));
        return arr;
    }
    vector<vector<string>> split2(const string &str, const string &chars1, const string &chars2) {
        vector<vector<string>> res;
        auto split1_arr = split(str, chars1);
        for (auto &it : split1_arr) {
            res.push_back(split(it, chars2));
        }
        return res;
    }
}  // namespace hb::util
