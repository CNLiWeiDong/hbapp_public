#include <hb/crypto/random.h>
#include <iostream>
#include <boost/random/random_device.hpp>
#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"

namespace hb { namespace crypto {
    using namespace std;

    std::string random_str(unsigned len)
    {
        std::string chars(
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "1234567890");
            // "!@#$%^&*()"
            // "`~-_=+[{]}\\|;:'\",<.>/? ");
        boost::random::random_device rng;
        boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
        string digest;
        for(int i = 0; i < len; ++i) {
           digest.push_back(chars[index_dist(rng)]);
        }
        return digest;
    }
    unsigned random_num(unsigned limit_max){
        if (limit_max<=1) return 0;
        boost::random::random_device rng;
        boost::random::uniform_int_distribution<> index_dist(0, limit_max-1);
        return index_dist(rng);
    }

} } // namespace hb
