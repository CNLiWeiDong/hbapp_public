#define BOOST_TEST_MODULE cypher_suites
#include <hb/crypto/aes.h>
#include <hb/crypto/base.h>
#include <hb/crypto/hex.h>
#include <hb/crypto/md5.h>
#include <hb/crypto/rsa.h>
#include <hb/crypto/sha.h>
#include <hb/http/http.h>
#include <hb/https/https.h>
#include <hb/log/log.h>
#include <hb/zip/zlib.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <list>
#include <string>

using namespace hb::crypto;
using namespace boost::property_tree;

BOOST_AUTO_TEST_SUITE(cypher_suites)
// test_log
BOOST_AUTO_TEST_CASE(test_log) try {
    log_trace("log test %s", "ok");
    log_debug("log test  %s", "ok");
    log_info("log test %s", "ok");
    log_warn("log test %s", "ok");
    log_error("log test  %s", "ok");
    log_fatal("log test  %s", "ok");
    BOOST_CHECK_EQUAL(true, true);
} catch (...) {
    log_error("test_log error!");
};

BOOST_AUTO_TEST_CASE(test_list) try {
    list<int> m_animtStepList = {1, 2, 3, 4};
    int from = 1, to = 3;
    if (from < 0 || from >= m_animtStepList.size() || to < 0 || to >= m_animtStepList.size()) {
        return;
    }
    auto formIter = m_animtStepList.begin();
    std::advance(formIter, from);

    auto toIter = m_animtStepList.begin();
    std::advance(formIter, to);

    m_animtStepList.splice(toIter, m_animtStepList, formIter);

    for (auto &it : m_animtStepList) {
        LOG_INFO("log test %d", it);
    }
} catch (...) {
    log_error("test_log error!");
};

// test_zip
BOOST_AUTO_TEST_CASE(test_zip) try {
    {
        string source_str("1234567890");
        hb::type::bytes source(source_str.begin(), source_str.end());
        auto zip_ret = hb::zip::zlib_compress(source);
        auto unzip_str = hb::zip::zlib_decompress(zip_ret);
        BOOST_CHECK_EQUAL(source_str, string(unzip_str.begin(), unzip_str.end()));
    }
    {
        string source_str(
            "1234567890123456789012345678901234567890123456789012345678901234567890"
            "1234567890123456789012345678901234567890123456789012345678901234567890"
            "1234567890123456789012345678901234567890123456789012345678901234567890"
            "1234567890123456789012345678901234567890123456789012345678901234567890"
            "12345678901234567890");
        hb::type::bytes source(source_str.begin(), source_str.end());
        auto zip_ret = hb::zip::zlib_compress(source);
        auto unzip_str = hb::zip::zlib_decompress(zip_ret);
        BOOST_CHECK_EQUAL(source_str, string(unzip_str.begin(), unzip_str.end()));
    }

} catch (...) {
    log_error("test_zip error!");
};
// test_aes
BOOST_AUTO_TEST_CASE(test_aes) try {
    std::string source_str(
        "123456789012345678901234567890123456789012345678901234567890123456789012"
        "345678901234567890123456789012345678901234567890123456789012345678901234"
        "567890123456789012345678901234567890123456789012345678901234567890123456"
        "789012345678901234567890123456789012345678901234567890123456789012345678"
        "901234567890");

    auto encrypt_data = cfb_aes_encrypt(sha512("123456"), source_str);
    auto decrypt_data = cfb_aes_decrypt(sha512("123456"), encrypt_data);
    BOOST_CHECK_EQUAL(source_str, decrypt_data);
} catch (...) {
    log_error("test_aes error!");
};

// test_ecb_256_aes
BOOST_AUTO_TEST_CASE(test_ecb_256_aes) try {
    std::string source_str = "test";
    auto encrypt_data = ecb_256_aes_encrypt(hex(md5("123456")), source_str);
    auto decrypt_data = ecb_256_aes_decrypt(hex(md5("123456")), encrypt_data);
    log_info("ecb_256_aes_encrypt key value: %s", hex(md5("123456")).c_str());
    log_info("ecb_256_aes_encrypt [test] value: %s", encrypt_data.c_str());
    log_info("ecb_256_aes_decrypt decrypt_data: %s", decrypt_data.c_str());
    boost::trim_if(decrypt_data, boost::is_any_of(" \0"));  // "/a/b/c 变成a/b/c"
    BOOST_CHECK_EQUAL(source_str, decrypt_data);
} catch (...) {
    log_error("test_ecb_256_aes error!");
};

// test_ecb_256_aes2
BOOST_AUTO_TEST_CASE(test_ecb_256_aes2) try {
    // std::string source_str = "test";
    // auto encrypt_data = ecb_256_aes_encrypt(hex(md5("123456")), source_str);
    auto decrypt_data = ecb_256_aes_decrypt("c829a6948aa3d614f1bc26e39f58ab06",
                                            "FWs+6CI/i6p7Vh7In/utxhamb+6xnB4QnUxkMEF6UrA=");

    log_info("test_ecb_256_aes2 decrypt_data: %s", decrypt_data.c_str());
} catch (...) {
    log_error("test_ecb_256_aes2 error!");
};

// test_base64
BOOST_AUTO_TEST_CASE(test_base64) try {
    std::string source_str(
        "123456789012345678901234567890123456789012345678901234567890123456789012"
        "345678901234567890123456789012345678901234567890123456789012345678901234"
        "567890123456789012345678901234567890123456789012345678901234567890123456"
        "789012345678901234567890123456789012345678901234567890123456789012345678"
        "901234567890");
    auto encrypt_data = base64(source_str);
    auto decrypt_data = base64_to_str(encrypt_data);
    BOOST_CHECK_EQUAL(source_str, decrypt_data);
} catch (...) {
    log_error("test_base64 error!");
};
// test_hex
BOOST_AUTO_TEST_CASE(test_hex) try {
    std::string source_str(
        "123456789012345678901234567890123456789012345678901234567890123456789012"
        "345678901234567890123456789012345678901234567890123456789012345678901234"
        "567890123456789012345678901234567890123456789012345678901234567890123456"
        "789012345678901234567890123456789012345678901234567890123456789012345678"
        "901234567890");
    auto encrypt_data = hex(source_str);
    auto decrypt_data = hex_to_str(encrypt_data);
    BOOST_CHECK_EQUAL(source_str, string(decrypt_data.begin(), decrypt_data.end()));
} catch (...) {
    log_error("test_hex error!");
};
// test_md5
BOOST_AUTO_TEST_CASE(test_md5) try {
    std::string source_str1("123456");
    std::string source_str2(
        "123456789012345678901234567890123456789012345678901234567890123456789012"
        "345678901234567890123456789012345678901234567890123456789012345678901234"
        "567890123456789012345678901234567890123456789012345678901234567890123456"
        "789012345678901234567890123456789012345678901234567890123456789012345678"
        "901234567890");
    auto md5_str1 = md5(source_str1);
    log_info("md5 sum:===========%s", hex(md5_str1).c_str());
    auto md5_str2 = md5(source_str2);
    BOOST_CHECK_EQUAL(true, md5_str1 != md5_str2);
} catch (...) {
    log_error("test_md5 error!");
};

// test_sha1
BOOST_AUTO_TEST_CASE(test_sha1) try {
    BOOST_CHECK_EQUAL(true, sha1("123456") != sha1("1234567"));
} catch (...) {
    log_error("test_sha1 error!");
};
// test_sha256
BOOST_AUTO_TEST_CASE(test_sha256) try {
    BOOST_CHECK_EQUAL(true, sha256("123456") != sha256("1234567"));
} catch (...) {
    log_error("test_sha256 error!");
};
// test_sha512
BOOST_AUTO_TEST_CASE(test_sha512) try {
    BOOST_CHECK_EQUAL(true, sha512("123456") != sha512("1234567"));
} catch (...) {
    log_error("test_sha512 error!");
};

// test_rsa
BOOST_AUTO_TEST_CASE(test_rsa) try {
    std::string strPri, strPub;
    create_rsa_key(strPri, strPub);
    std::string chiper = rsa_encrypt(strPub, "test");
    BOOST_CHECK_EQUAL(rsa_decrypt(strPri, chiper), "test");
    std::string sig = sign_msg(strPri, "test");
    log_info("sig:===========");
    log_info("%s", base64(sig).c_str());
    verify_msg(strPub, "test", sig);

} catch (...) {
    log_error("test_rsa error!");
};

// test_rsa_verify_pub
BOOST_AUTO_TEST_CASE(test_rsa_verify_pub) try {
    std::string strPub = pem_str_to_rsa_pub(R"(-----BEGIN PUBLIC KEY-----
MIIBoDANBgkqhkiG9w0BAQEFAAOCAY0AMIIBiAKCAYEA4zeYp54c0BZRHHycEaPL
r1pKDAby0EN5BU9wZFbWNZ/je26LynPmKO2wlWe2SwGUojGDLS02yS92j627f+wK
MPVGR29bj3O8YeCEDZeN5lbYUoZ/thX+g9vFZdrZhzYfJr8t1k/2WbbuRZtWZs8/
7DtQnw9LeJgAODNYYWBoRAUY9PfdR2DcKqtyvArF1sUMBHRQv1nW1sZIoG5DEe38
s9G3sBAa2agi2kd/lR8GdBFNOpyYvUUIu80owqDXH1GVRpe3UEwVTyKojXBgLnnH
zNgUbF/CwYXRwNmvYUno601vauTyPv3kli2CetD63FOMDqW9ObYQtwvSML0S9Mxx
eQ1ypqog+6czKHtTSHvw4rOpKK2lUd6nxkkpyY4ZbZrJGRUi9IX2R/FbbwRP6Hwl
E2x7EFacmI7YZqzlfbICoogrHnQ6X4WEAlOwLNm2FP3F5SV98mILlCw9LU5CdjHq
V2jaAfCYkdgb3BgiDisbKSRhodOs9fCljqIBpDa1WCIdAgER
-----END PUBLIC KEY-----)");

    std::string sig = base64_to_str(
        R"(IaiywKnQJ/DFXQ2DAll6gq1ydSZYWJIBaZWbM6jWOSC2yyZh0pgx6WaAcGTTccsbb/NEs6mgBWeB
l6565uAgWVJJB+1aE+0iXUAuNCj41kgEAW0RCeb2JDzz1qJYgpUVwPAcsXOh+zNT0lKZ5bRskxGl
Isydq7zANc7h4knOlnTschFyt7q6PliBjfFYz1NsQqemkECcPmBzCMdLUkfwgbNB9IjB2MBhbDJr
GwDUnheAtr+fVwePeeS03dzzO39MLena3WVr4/YABOP3zRhc6oagDm/9BxHZNCy7+NZLB1I3YyAy
Qu6GicksruZ9OJyMC0JBMwii+j4va6cwyu7jJK+VGzZbdhyhk72Y3JXKPpej3AzoAJ2VWhaUZtEP
6S6XSnz4aBF3JeHCax7SCudYi/6MMgRYGSheVd6T5u7JxsaJy0aRbp2iTlEcsk2CZl6M0WHJOCwi
KV44gNpFTTkArdddFTONKSuniqmWzDM7cPFkEemIoomhG8hnc26C/IPJ)");
    verify_msg(strPub, "/block/query/userblock1636473734", sig);
    log_info("test_rsa_verify_pub success!");

} catch (...) {
    log_error("test_rsa_verify_pub error!");
};

// test_rsa_verify_sign_game_client
BOOST_AUTO_TEST_CASE(test_rsa_verify_sign_game_client) try {
    std::string strPub = pem_str_to_rsa_pub(R"(-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCaWq5LDl5LhRVJVXL+sXDKaWMP
1jWa5SF/IhJgnayknrel+ArhahDUGOPV6l+WhpjIsNuvbLxwSOEFDqBZRFz+zbTZ
g6oxYe17lbxSJu3hZqmz58EuTCkgAL8DEmKIKUyvgdG16GoCLgMtd2GekcfQ2+Z4
LUA7iD8bbGW4q+bvZwIDAQAB
-----END PUBLIC KEY-----)");

    std::string sig = base64_to_str(
        R"(KW6M4jqPpeSwmaBkL26ZIZklwjLp/NIasafn675EPX/w1p1L2VsgPixf+UjFcfBOMqxe8w3dzww1fTxbMNLUDY5fTRLO4u7HU0nqNTTCi4BQdH/GZ/c/kEInK6C0g8tcn0EX9NoUpqdNPzA6/lKDwK4xhJbvhL2s+ebzuLiYzXQ=)");
    verify_msg(strPub, "1234567890", sig);
    log_error("test_rsa_verify_sign_game_client success!");

} catch (...) {
    log_error("test_rsa_verify_sign_game_client error!");
};

// test_rsa_verify_keys
BOOST_AUTO_TEST_CASE(test_rsa_verify_keys) try {
    std::string strPub = pem_str_to_rsa_pub(R"(-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCaWq5LDl5LhRVJVXL+sXDKaWMP
1jWa5SF/IhJgnayknrel+ArhahDUGOPV6l+WhpjIsNuvbLxwSOEFDqBZRFz+zbTZ
g6oxYe17lbxSJu3hZqmz58EuTCkgAL8DEmKIKUyvgdG16GoCLgMtd2GekcfQ2+Z4
LUA7iD8bbGW4q+bvZwIDAQAB
-----END PUBLIC KEY-----)");

    std::string strPri = pem_str_to_rsa_pri(R"(-----BEGIN PRIVATE KEY-----
MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAJparksOXkuFFUlV
cv6xcMppYw/WNZrlIX8iEmCdrKSet6X4CuFqENQY49XqX5aGmMiw269svHBI4QUO
oFlEXP7NtNmDqjFh7XuVvFIm7eFmqbPnwS5MKSAAvwMSYogpTK+B0bXoagIuAy13
YZ6Rx9Db5ngtQDuIPxtsZbir5u9nAgMBAAECgYAc5W8NtBHpF3AFu458qeFQs2wN
Me/1IIIl184S8lG4u9Hrq0T2MDIYUtSMXoUom+gMYjeuXq/kYXBmzvcr3wV9j0MK
EMe2uI7FENnQWfh5xazHzvlj+vTEBumNn8iM0SXhb/Yogp4Z8UueOArA8GFVcEYX
Pa8YXm9J3DkSdoFTCQJBALF+VhWDj9qDLnonr+Si1de4gLkU3s/8sAiCKRVdDJuu
VOOuIP67EgRjoj2hTplBhS0hB89cMEzGoEAt53BEveUCQQDeoEY24e5UXiT4YgJ5
hyoPi9MkofD06D+8Mf1+nS8Q8/QLtnHdAVMerY4HVyvJfOOXqXfiETzW8oSSi1lO
asNbAkB2BldByREQMWjna5qHw0vFPXfDGsCqtxFrJvogrZYjs4HZQEHKgSSS7JnG
t4unVy0yfCNQXLH1a6LF6oBaZCuNAkAoqxaoh/VFrD0MsrrJAIwKcvyLQJoH4zVh
Z7BrJMOoUShPy3BZcxiADixX4vC4yfu5hNfFkGiQy5S0QIh77rzpAkB8hvH2X9FT
NQ7ozV/0f5ldg+T1m983i7L/IySmQaWhmeQWpMmlCYbQZZSd/A1LWY+krwJa8AFG
BC9Eub2fXHis
-----END PRIVATE KEY-----)");

    std::string chiper = rsa_encrypt(strPub, "test");
    BOOST_CHECK_EQUAL(rsa_decrypt(strPri, chiper), "test");
    std::string sig = sign_msg(strPri, "test");
    log_info("sig22222222:===========");
    log_info(base64(sig).c_str());
    verify_msg(strPub, "test", sig);
    log_info("test_rsa_verify_keys success!");

} catch (...) {
    log_error("test_rsa_verify_keys error!");
};
// test_pem
BOOST_AUTO_TEST_CASE(test_pem) try {
    std::string strPri, strPub;
    create_rsa_key(strPri, strPub);
    sava_rsa_pem_pri("test_rsa_pri.pem", strPri, "123456");
    BOOST_CHECK_EQUAL(read_rsa_pem_pri("test_rsa_pri.pem", "123456"), strPri);

    sava_rsa_pem_pub("test_rsa_pub.pem", strPub);
    BOOST_CHECK_EQUAL(read_rsa_pem_pub("test_rsa_pub.pem"), strPub);

    cout << std::endl;
    cout << rsa_pri_to_pem_str(strPri) << std::endl;

    cout << "pass" << std::endl;
    cout << rsa_pri_to_pem_str(strPri, "123456") << std::endl;

    cout << std::endl;
    cout << rsa_pub_to_pem_str(strPub) << std::endl;

    BOOST_CHECK_EQUAL(pem_str_to_rsa_pri(rsa_pri_to_pem_str(strPri)), strPri);
    BOOST_CHECK_EQUAL(pem_str_to_rsa_pub(rsa_pub_to_pem_str(strPub)), strPub);

} catch (...) {
    log_error("test_pem error!");
};

// test_http_get
BOOST_AUTO_TEST_CASE(test_http_get) try {
    hb::http::http h("baidu.com", "80", "/");
    auto status = h.get();
    log_info("baidu.com get: %d %s", status, h.res_body().c_str());
} catch (...) {
    log_error("test_http_get error!");
};
// test_http_post
BOOST_AUTO_TEST_CASE(test_http_post) try {
    hb::http::http h("baidu.com", "80", "/");
    auto status = h.post();
    log_info("baidu.com post: %d %s", status, h.res_body().c_str());
} catch (...) {
    log_error("test_http_post error!");
};
// test_http_async_get
BOOST_AUTO_TEST_CASE(test_http_async_get) try {
    hb::http::http h("baidu.com", "80", "/");
    boost::asio::io_context ioc;
    h.get(ioc, [](const int &status, const std::string &res) {
        log_info("baidu.com async_get: %d %s", status, res.c_str());
    });
    ioc.run();

} catch (...) {
    log_error("test_http_async_get error!");
};

// test_http_async_post
BOOST_AUTO_TEST_CASE(test_http_async_post) try {
    hb::http::http h("baidu.com", "80", "/");
    boost::asio::io_context ioc;
    h.post(ioc, [](const int &status, const std::string &res) {
        log_info("baidu.com async_post: %d %s", status, res.c_str());
    });
    ioc.run();

} catch (...) {
    log_error("test_http_async_post error!");
};

const std::string cert_str = "";
// test_https_get
BOOST_AUTO_TEST_CASE(test_https_get) try {
    using namespace hb::https;
    hb::https::https hs("xueqiu.com", "443", "/S/SH513500");
    // shell :  openssl s_client  -connect www.baidu.com:80 -showcerts
    // auto ctx = make_shared<ssl::context>(ssl::context::tlsv12_client);
    // ctx->set_verify_mode(ssl::verify_none);
    // hs.ctx(ctx);
    auto status = hs.get();
    log_info("test_https_get https get: %d, %s", status, hs.res_body());
} catch (...) {
    log_throw("https_test", nullptr);
    log_error("test_https_get error!");
};
// test_https_async_post
BOOST_AUTO_TEST_CASE(test_https_async_post) try {
    using namespace hb::https;
    hb::https::https hs("exampleserver.com", "443", "/");
    auto ctx = make_shared<ssl::context>(ssl::context::tlsv12_client);
    hs.ctx(ctx);
    boost::asio::io_context ioc;
    hs.post(ioc, [](const int &status, const std::string &res) {
        log_info("exampleserver.com async_post: %d %s", status, res);
    });
    ioc.run();

} catch (...) {
    log_throw("test_https_async_post test", nullptr);
    log_error("test_https_async_post error!");
};

// json_parse
BOOST_AUTO_TEST_CASE(test_json_parse) try {
    ptree json_body;
    stringstream json_stream("{\"val\": true}");
    read_json(json_stream, json_body);
    log_info("bool-value:%s",json_body.get("val",""));
    BOOST_CHECK_EQUAL(json_body.get("val",""), "true");
    
} catch (...) {
    log_throw("test_json_parse test", nullptr);
    log_error("test_json_parse error!");
};
BOOST_AUTO_TEST_SUITE_END()