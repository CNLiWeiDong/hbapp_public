#pragma once

#include <hb/types/types.h>

#include <string>
#include <vector>

using namespace std;

namespace hb::util {

    vector<string> split(const string &str, const string &chars);
    vector<vector<string>> split2(const string &str, const string &chars1, const string &chars2);
    bool ignore_case_equal(const string &str1, const string &str2);
}  // namespace hb::util
