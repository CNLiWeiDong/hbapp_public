#define BOOST_TEST_MODULE msg_security_test

#include <hb/hb.h>
#include <hb/log_plugin/log_plugin.h>
#include <hb/msg_security_plugin/msg_security_plugin.h>

#include <appbase/application.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/test/unit_test.hpp>

using namespace appbase;
using namespace hb::plugin;

struct app_fixture {
    app_fixture() {
        auto exePath = boost::filesystem::initial_path<boost::filesystem::path>();
        app().set_default_config_dir(exePath / "config");
        app().set_default_data_dir(exePath / "data");
        char* name = "msg_security_test";
        char* arg1 = "--http-security-rsa-private-pass";
        char* arg2 = "123456";
        char* args[] = {name, arg1, arg2};
        if (!app().initialize<log_plugin, msg_security_plugin>(3, args)) return;
        printf("msg_security_test app start %s\n", app().version_string().c_str());
        app().startup();
    }
    ~app_fixture() { std::cout << "suit teardown\n"; }
};
BOOST_TEST_GLOBAL_FIXTURE(app_fixture);

BOOST_AUTO_TEST_SUITE(s_msg_security)

// test_msg_security
BOOST_AUTO_TEST_CASE(test_msg_security) {
    hb_try {
        auto& msg_plugin = app().get_plugin<msg_security_plugin>();
        string chiper;
        string encrypted_msg;
        string msg;

        msg_plugin.encrypt(R"({"jsonrpc":"2.0","id":1,"to":"2686465170@qq.com","title":"Hello","content":"<p>Hello, chuidylan.</p >\n<p>Hello, the Most Honorable chuidylan.</p >"})", chiper, encrypted_msg);
        msg_plugin.decrypt(encrypted_msg, chiper, msg);
        log_info("test: encrypted_msg:%s", encrypted_msg.c_str());
        log_info("chiper:%s", chiper.c_str());
        log_info("encrypt and decrypt success!");
        log_info("msg:%s", msg.c_str());
        // boost::trim_if(msg, boost::is_any_of(" \0"));
        // log_info("msg2:%s", msg.c_str());
        log_info("====================================================================");
    }
    hb_catch([&](const auto& e) { log_throw("msg_security_test error:", e); });
}

// test_msg_security_decode
BOOST_AUTO_TEST_CASE(test_msg_security_decode) {
    hb_try {
        auto& msg_plugin = app().get_plugin<msg_security_plugin>();
        string chiper
            = "dT3qCVcqNZYNj+B4yxHkcqty0Qr5wNIp96j3r8j9aBfXXwdhN7ezF9d4RMqnunR4McVPDqKSie+3uiz0gBl1y1nmbKDwfJgElVd4RcdBzQtS0pOYQBaQa15n8gox61W5MFj8TT5dA7XQtSFuNuXUszwpMEEeVPmW9aKs7zInBvBFNxt8n2kpedklqmQTrnSi3K1SZexNOGY01QvC6v07+r3INf9BdXyghOQed5ZeeLqxS85KKEvSlbdiey1wOW+NVPKclh5HFpvtZBmi0nX+r+AJektgnQ98N2wIG1Fd6+NZ86s4bt1Ni7NeBYmBUXlDaDYRE/py34/R23CnBQ5HHkXbAmx9zzJvKBH+8GeiH2p8xOPwYje56ywWjSwyVggbbYfXb1FRlBdbzEuqy8vwPNqYoQ3ovcF/hwuKYzrmhy+8qoo9bdEaIFQe3L/Wz91zgR1ggZrgqvatPZvm8ut3815Xf3AB5j85teKbLEeb7gnRjeknLI68V57OCn95qtpF";
        string encrypted_msg = "JQJusg2fq7s+VcxJN3zrSUnNAnzTBtJaBnMfZzGJWLeVez0MFDeReSUCWMUyaB0DURaAjoRQXk6ycwxrkK3NfNdXQVrQ8yUuriudfaQDy4sH2wAn8i480FvFlXSLv9QDMQ817/wu7eqZuyEL/CdzJeqm69cdo6u24BvG9X1jYjKtX/dAhQfXB1GaMrQ3x7yI3oWhLF+iuepGUiST4s0SRQ==";
        string msg;

        msg_plugin.decrypt(encrypted_msg, chiper, msg);
        log_info("msg:%s", msg.c_str());
        log_info("msg-l:%d", msg.size());
        log_info("msg_hex:%s", hb::crypto::hex(msg).c_str());
        log_info("====================================================================");
    }
    hb_catch([&](const auto& e) { log_throw("msg_security_test error:", e); });
}

// // test_msg_security_decode1
// BOOST_AUTO_TEST_CASE(test_msg_security_decode1) {
//     hb_try {
//         log_info("====================================================================");

//         auto& msg_plugin = app().get_plugin<msg_security_plugin>();
//         string chiper
//             = "WrWAkVnjCjNGa65/32OP5ya1YWKk/"
//               "Qzrmz5BL+HvpXvFTH9K1UNKPOg8F+K7evfutjped7APzcCUsFqya+dikVJXmb8Xq/"
//               "+sOX0cjqhmAZxmuYnh67qF+QnCIRG5AoimUrKvJm0gvF/MKh7/"
//               "ggN1+"
//               "axrB9P0aJPGbsGFDEn8225FOp1VbMgcbu3SRuqgajOLdcHBvJwwKJs8Esc3rmjYhhxqrGlkYPMgiG8aU8MmH"
//               "4QMiHoaY7fN6zUGm4KZvsDFyUNQdfaHe+o6a740nt1THzAz6rImZAsc7BtUxcnVL/"
//               "G9PZRxqQL3yAwr4R0a524Q80GnU4N5L1DbMEJ8JJUySLZaRV2SkheUO4VcsBTGH279PKHIQC64EsuAw0Wy2D"
//               "C60+"
//               "SfDFWHYU15z620jxWqbgdAtiiPS42YhGG3aUzEWoK7DWbI2EYFm3kZSsPrtNL7HOZTcy19EfKhaoSNkUHmJI"
//               "VRCUfinfsTzFHksjAD7dqfCq5Y8L9QgbOAyeeP9zUE";
//         string encrypted_msg = "FWs+6CI/i6p7Vh7In/utxhamb+6xnB4QnUxkMEF6UrA=";
//         string msg;

//         msg_plugin.decrypt(encrypted_msg, chiper, msg);

//         log_info("msg:%s", msg.c_str());
//         log_info("msg_hex:%s", hb::crypto::hex(msg).c_str());

//         boost::trim_if(msg, boost::is_any_of(" \0"));
//         log_info("msg_hex:%s", hb::crypto::hex(msg).c_str());
//         msg = msg.c_str();
//         log_info("msg_cstr:%s", hb::crypto::hex(msg).c_str());
//         boost::property_tree::ptree j;
//         stringstream stream(msg);
//         read_json(stream, j);
//         log_info(to_string(j.get("a", 0)));
//     }
//     hb_catch([&](const auto& e) { log_throw("msg_security_test error:", e); });
// }
BOOST_AUTO_TEST_SUITE_END()