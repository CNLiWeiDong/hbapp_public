#define BOOST_TEST_MODULE msg_security_test

#include <boost/test/unit_test.hpp>
#include <hb/hb.h>
#include <appbase/application.hpp>
#include <hb/msg_security_plugin/msg_security_plugin.h>
#include <hb/log_plugin/log_plugin.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using namespace appbase;
using namespace hb::plugin;

struct  app_fixture
{
    app_fixture() { 
        auto exePath = boost::filesystem::initial_path<boost::filesystem::path>();
        app().set_default_config_dir(exePath / "config");
        app().set_default_data_dir(exePath / "data");
        char* name = "msg_security_test";
        char* arg1 = "--http-security-rsa-private-pass";
        char* arg2 = "123456";
        char* args[] = { name, arg1, arg2 };
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
    try {
        auto &msg_plugin = app().get_plugin<msg_security_plugin>();
        string chiper;
        string encrypted_msg;
        string msg;

        msg_plugin.encrypt("hello",chiper, encrypted_msg);
        msg_plugin.decrypt(encrypted_msg, chiper, msg);
        log_info<<"encrypt and decrypt success!";
    } catch (...) {
        log_error << "msg_security_test error!";
    }
}
BOOST_AUTO_TEST_SUITE_END()