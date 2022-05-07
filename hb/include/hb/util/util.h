#pragma once

#include <hb/types/types.h>
#include <boost/format.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace hb::util {

    vector<string> split(const string &str, const string &chars);
    vector<vector<string>> split2(const string &str, const string &chars1, const string &chars2);
    bool ignore_case_equal(const string &str1, const string &str2);

    template<typename... Arguments>
    std::string format_args(const std::string& fmt, const Arguments&... args)
    {
        return boost::str((boost::format(fmt) % ... % args));
    }
}  // namespace hb::util
