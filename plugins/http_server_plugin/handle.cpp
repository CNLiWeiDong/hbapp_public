#include <hb/http_server_plugin/handle.h>
#include <boost/algorithm/string.hpp>


namespace hb { namespace http_server { 

     map<string, shared_ptr<signal_type>> registed_signals_;
    mutex handle::signals_mutex_;

    shared_ptr<signal_type> handle::regist(const string &target){
        std::unique_lock<std::mutex> lock(signals_mutex_); 
        auto it = registed_signals_.find(target);
        if(it!=registed_signals_.end())
            return it->second;
        shared_ptr<signal_type> sig = make_shared<signal_type>();
        registed_signals_.insert(decltype(registed_signals_)::value_type(target,std::move(sig)));
        return registed_signals_[target];
    }
    void handle::connect(const string &target, deal_fun fun){
        auto sig = regist(target);
        sig->connect(fun);
    }
    vector<string> handle::split_target(const string& target) {
        string trim_target = boost::trim_copy_if(target,boost::is_any_of("/_-\t "));
        vector<string> target_split;
        boost::split(target_split, trim_target, boost::is_any_of("/_-"));

        vector<string> targets;
        // target: a/b/c  拆分成"a","a-b","a-b-c"一共3个target, num是target的数量
        for (int num=1; num<=target_split.size(); num++) {
            string t;
            for (int i=0; i<num; i++) {
                t += target_split[i]+"-";
            }
            boost::trim_if(t,boost::is_any_of("-\t ")); 
            targets.push_back(boost::to_lower_copy(t));
        }
        return targets;
    }
    template<class REQ_TYPE>
    signal_results handle::deal_request(const REQ_TYPE &&req){
        auto targets = split_target(req.target);
        signal_results results;
        for(auto &target : targets) {
            shared_ptr<signal_type> sig;
            {
                std::unique_lock<std::mutex> lock(signals_mutex_);
                decltype(registed_signals_.end()) it = registed_signals_.find(target);
                if(it != registed_signals_.end()){
                    sig = it->second;
                }
            }
            if (sig && sig->num_slots()>0) {
                auto one_result = sig(req.body());
                results.insert(results.end(),one_result.begin(),one_result.end());
            }
        }
        return results;
    }
    
    template<class REQ_TYPE, class Send>
    void handle::request(const REQ_TYPE&& req, Send&& send) {
        ptree pt_res;
        pt_res.put("error","ok");
        pt_res.put("type","string-array");
        pt_res.put("target",req.target);
        ptree results;
        
        auto const send_response = [&](unsigned status){
            pt_res.put_child("results",results);
            stringstream stream;
            write_json(stream,pt_res);
            http::response<http::string_body> res{status, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/text");
            res.keep_alive(req.keep_alive());
            res.body() = std::string(stream.str());
            res.prepare_payload();
            send(std::move(res));
        };
        hb_try
            auto deal_results = deal_request(req);
            if(deal_results.size()==0){
                pt_res.put("error","Target has no corresponding processing method!");
                send_response(http::status::bad_request);
                return;
            }
            for(auto &item : deal_results){
                results.push_back(std::make_pair("",item));
            }
            send_response(http::status::ok);
        hb_catch([&](const auto &e){
            pt_res.put("error",log_throw("do handle_request work error", e));
            send_response(http::status::internal_server_error);
        })
    }

    /*  Load a signed certificate into the ssl context, and configure
        the context for use with a server.

        For this to work with the browser or operating system, it is
        necessary to import the "Beast Test CA" certificate into
        the local certificate store, browser, or operating system
        depending on your environment Please see the documentation
        accompanying the Beast certificate for more details.
    */
    void load_server_certificate(boost::asio::ssl::context& ctx)
    {
        /*
            The certificate was generated from CMD.EXE on Windows 10 using:

            winpty openssl dhparam -out dh.pem 2048
            winpty openssl req -newkey rsa:2048 -nodes -keyout key.pem -x509 -days 10000 -out cert.pem -subj "//C=US\ST=CA\L=Los Angeles\O=Beast\CN=www.example.com"
        */

        std::string const cert =
            "-----BEGIN CERTIFICATE-----\n"
            "MIIDaDCCAlCgAwIBAgIJAO8vBu8i8exWMA0GCSqGSIb3DQEBCwUAMEkxCzAJBgNV\n"
            "BAYTAlVTMQswCQYDVQQIDAJDQTEtMCsGA1UEBwwkTG9zIEFuZ2VsZXNPPUJlYXN0\n"
            "Q049d3d3LmV4YW1wbGUuY29tMB4XDTE3MDUwMzE4MzkxMloXDTQ0MDkxODE4Mzkx\n"
            "MlowSTELMAkGA1UEBhMCVVMxCzAJBgNVBAgMAkNBMS0wKwYDVQQHDCRMb3MgQW5n\n"
            "ZWxlc089QmVhc3RDTj13d3cuZXhhbXBsZS5jb20wggEiMA0GCSqGSIb3DQEBAQUA\n"
            "A4IBDwAwggEKAoIBAQDJ7BRKFO8fqmsEXw8v9YOVXyrQVsVbjSSGEs4Vzs4cJgcF\n"
            "xqGitbnLIrOgiJpRAPLy5MNcAXE1strVGfdEf7xMYSZ/4wOrxUyVw/Ltgsft8m7b\n"
            "Fu8TsCzO6XrxpnVtWk506YZ7ToTa5UjHfBi2+pWTxbpN12UhiZNUcrRsqTFW+6fO\n"
            "9d7xm5wlaZG8cMdg0cO1bhkz45JSl3wWKIES7t3EfKePZbNlQ5hPy7Pd5JTmdGBp\n"
            "yY8anC8u4LPbmgW0/U31PH0rRVfGcBbZsAoQw5Tc5dnb6N2GEIbq3ehSfdDHGnrv\n"
            "enu2tOK9Qx6GEzXh3sekZkxcgh+NlIxCNxu//Dk9AgMBAAGjUzBRMB0GA1UdDgQW\n"
            "BBTZh0N9Ne1OD7GBGJYz4PNESHuXezAfBgNVHSMEGDAWgBTZh0N9Ne1OD7GBGJYz\n"
            "4PNESHuXezAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQCmTJVT\n"
            "LH5Cru1vXtzb3N9dyolcVH82xFVwPewArchgq+CEkajOU9bnzCqvhM4CryBb4cUs\n"
            "gqXWp85hAh55uBOqXb2yyESEleMCJEiVTwm/m26FdONvEGptsiCmF5Gxi0YRtn8N\n"
            "V+KhrQaAyLrLdPYI7TrwAOisq2I1cD0mt+xgwuv/654Rl3IhOMx+fKWKJ9qLAiaE\n"
            "fQyshjlPP9mYVxWOxqctUdQ8UnsUKKGEUcVrA08i1OAnVKlPFjKBvk+r7jpsTPcr\n"
            "9pWXTO9JrYMML7d+XRSZA1n3856OqZDX4403+9FnXCvfcLZLLKTBvwwFgEFGpzjK\n"
            "UEVbkhd5qstF6qWK\n"
            "-----END CERTIFICATE-----\n";

        std::string const key =
            "-----BEGIN PRIVATE KEY-----\n"
            "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDJ7BRKFO8fqmsE\n"
            "Xw8v9YOVXyrQVsVbjSSGEs4Vzs4cJgcFxqGitbnLIrOgiJpRAPLy5MNcAXE1strV\n"
            "GfdEf7xMYSZ/4wOrxUyVw/Ltgsft8m7bFu8TsCzO6XrxpnVtWk506YZ7ToTa5UjH\n"
            "fBi2+pWTxbpN12UhiZNUcrRsqTFW+6fO9d7xm5wlaZG8cMdg0cO1bhkz45JSl3wW\n"
            "KIES7t3EfKePZbNlQ5hPy7Pd5JTmdGBpyY8anC8u4LPbmgW0/U31PH0rRVfGcBbZ\n"
            "sAoQw5Tc5dnb6N2GEIbq3ehSfdDHGnrvenu2tOK9Qx6GEzXh3sekZkxcgh+NlIxC\n"
            "Nxu//Dk9AgMBAAECggEBAK1gV8uETg4SdfE67f9v/5uyK0DYQH1ro4C7hNiUycTB\n"
            "oiYDd6YOA4m4MiQVJuuGtRR5+IR3eI1zFRMFSJs4UqYChNwqQGys7CVsKpplQOW+\n"
            "1BCqkH2HN/Ix5662Dv3mHJemLCKUON77IJKoq0/xuZ04mc9csykox6grFWB3pjXY\n"
            "OEn9U8pt5KNldWfpfAZ7xu9WfyvthGXlhfwKEetOuHfAQv7FF6s25UIEU6Hmnwp9\n"
            "VmYp2twfMGdztz/gfFjKOGxf92RG+FMSkyAPq/vhyB7oQWxa+vdBn6BSdsfn27Qs\n"
            "bTvXrGe4FYcbuw4WkAKTljZX7TUegkXiwFoSps0jegECgYEA7o5AcRTZVUmmSs8W\n"
            "PUHn89UEuDAMFVk7grG1bg8exLQSpugCykcqXt1WNrqB7x6nB+dbVANWNhSmhgCg\n"
            "VrV941vbx8ketqZ9YInSbGPWIU/tss3r8Yx2Ct3mQpvpGC6iGHzEc/NHJP8Efvh/\n"
            "CcUWmLjLGJYYeP5oNu5cncC3fXUCgYEA2LANATm0A6sFVGe3sSLO9un1brA4zlZE\n"
            "Hjd3KOZnMPt73B426qUOcw5B2wIS8GJsUES0P94pKg83oyzmoUV9vJpJLjHA4qmL\n"
            "CDAd6CjAmE5ea4dFdZwDDS8F9FntJMdPQJA9vq+JaeS+k7ds3+7oiNe+RUIHR1Sz\n"
            "VEAKh3Xw66kCgYB7KO/2Mchesu5qku2tZJhHF4QfP5cNcos511uO3bmJ3ln+16uR\n"
            "GRqz7Vu0V6f7dvzPJM/O2QYqV5D9f9dHzN2YgvU9+QSlUeFK9PyxPv3vJt/WP1//\n"
            "zf+nbpaRbwLxnCnNsKSQJFpnrE166/pSZfFbmZQpNlyeIuJU8czZGQTifQKBgHXe\n"
            "/pQGEZhVNab+bHwdFTxXdDzr+1qyrodJYLaM7uFES9InVXQ6qSuJO+WosSi2QXlA\n"
            "hlSfwwCwGnHXAPYFWSp5Owm34tbpp0mi8wHQ+UNgjhgsE2qwnTBUvgZ3zHpPORtD\n"
            "23KZBkTmO40bIEyIJ1IZGdWO32q79nkEBTY+v/lRAoGBAI1rbouFYPBrTYQ9kcjt\n"
            "1yfu4JF5MvO9JrHQ9tOwkqDmNCWx9xWXbgydsn/eFtuUMULWsG3lNjfst/Esb8ch\n"
            "k5cZd6pdJZa4/vhEwrYYSuEjMCnRb0lUsm7TsHxQrUd6Fi/mUuFU/haC0o0chLq7\n"
            "pVOUFq5mW8p0zbtfHbjkgxyF\n"
            "-----END PRIVATE KEY-----\n";

        std::string const dh =
            "-----BEGIN DH PARAMETERS-----\n"
            "MIIBCAKCAQEArzQc5mpm0Fs8yahDeySj31JZlwEphUdZ9StM2D8+Fo7TMduGtSi+\n"
            "/HRWVwHcTFAgrxVdm+dl474mOUqqaz4MpzIb6+6OVfWHbQJmXPepZKyu4LgUPvY/\n"
            "4q3/iDMjIS0fLOu/bLuObwU5ccZmDgfhmz1GanRlTQOiYRty3FiOATWZBRh6uv4u\n"
            "tff4A9Bm3V9tLx9S6djq31w31Gl7OQhryodW28kc16t9TvO1BzcV3HjRPwpe701X\n"
            "oEEZdnZWANkkpR/m/pfgdmGPU66S2sXMHgsliViQWpDCYeehrvFRHEdR9NV+XJfC\n"
            "QMUk26jPTIVTLfXmmwU0u8vUkpR7LQKkwwIBAg==\n"
            "-----END DH PARAMETERS-----\n";
        
        ctx.set_password_callback(
            [](std::size_t,
                boost::asio::ssl::context_base::password_purpose)
            {
                return "test";
            });

        ctx.set_options(
            boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::single_dh_use);

        ctx.use_certificate_chain(
            boost::asio::buffer(cert.data(), cert.size()));

        ctx.use_private_key(
            boost::asio::buffer(key.data(), key.size()),
            boost::asio::ssl::context::file_format::pem);

        ctx.use_tmp_dh(
            boost::asio::buffer(dh.data(), dh.size()));
    }
}}