#pragma once

#include <hb/types/types.h>
#include <string>
#include <vector>

using namespace std;

namespace hb::util { 

    vector<string> split(const string &str, const string &chars);

} // namespace hb
