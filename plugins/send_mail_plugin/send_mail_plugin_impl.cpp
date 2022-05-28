//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/send_mail_plugin/send_mail_plugin_impl.h>
#include <hb/send_mail_plugin/session.h>
// #include <hb/thread_pool_plugin/thread_pool_plugin.h>
#include <typeinfo>
#include <string>

namespace hb::plugin {
    // static uint64_t send_mail_task_id = 0;

    void send_mail_plugin_impl::send_mail(const string &to, const string &subject,
                                            const string &content) {
        log_info("start send_mail_plugin_impl::send_mail");
        // 只要一访问_host就报错，原因是rest_control_plugin的CMakeLists.txt没有添加send_mail_plugin的依赖库
        // cout << typeid(to).name() << endl;
        // cout << typeid(_host).name() << endl; 
        session sess;
        sess.host(_host);
        sess.port(_port);
        sess.user(_user);
        sess.pass(_pass);
        sess.to(to);
        sess.subject(subject);
        sess.content(content);
        sess.mail();
        log_info("end send_mail_plugin_impl::send_mail");


        // auto &thread_plugin = app().get_plugin<thread_pool_plugin>();
        // auto api = thread_plugin.get_api();
        // auto self = shared_from_this();
        // std::string task_id = "send_mail_" + to_string(send_mail_task_id++);
        // log_info("send mail to: %s thread task is: %s", to.c_str(), task_id.c_str());
        // api->post(task_id, [=]() {
        //     session sess;
        //     sess.host(self->host());
        //     sess.port(self->port());
        //     sess.user(self->user());
        //     sess.pass(self->pass());
        //     sess.to(to);
        //     sess.subject(subject);
        //     sess.content(content);
        //     sess.mail();
        // });
    }
}  // namespace hb