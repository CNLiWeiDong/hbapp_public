#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <vector>

namespace hb::type {

    using namespace std;

    typedef boost::multiprecision::int128_t int128_t;
    typedef boost::multiprecision::uint128_t uint128_t;
    typedef boost::multiprecision::int256_t int256_t;
    typedef boost::multiprecision::uint256_t uint256_t;
    typedef boost::multiprecision::int512_t int512_t;
    typedef boost::multiprecision::uint512_t uint512_t;
    typedef boost::multiprecision::int1024_t int1024_t;
    typedef boost::multiprecision::uint1024_t uint1024_t;
    typedef boost::multiprecision::cpp_int cpp_int;
    using bytes = vector<char>;

}  // namespace hb::type