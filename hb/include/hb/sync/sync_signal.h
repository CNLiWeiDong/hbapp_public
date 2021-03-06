//
// Created by 李卫东 on 2019-02-25.
//

#ifndef CPPSERVER_SIGNAL_H
#define CPPSERVER_SIGNAL_H

#include <errno.h>
#include <fcntl.h>
#include <hb/error/exception.h>
#include <hb/sync/sync_error.h>
#include <stdio.h>
#include <unistd.h>

#include <atomic>
#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <vector>

#include "hb/log/log.h"

using namespace std;
//重新设计一版本。实现>> <<重载。模板可实现任意类型发送。
namespace hb {
    namespace sycn {
        class sync_signal {
          public:
            sync_signal(bool is_time_out = true, int sec = 30)
                : time_out_check(is_time_out), time_out_sec(sec) {
                if (::pipe(_pipe_sign) != 0)
                    hb_throw(hb_sync_exception().msg("init sync_signal pipe sign error!"));
                ::fcntl(_pipe_sign[0], F_SETFL, O_NONBLOCK);
                ::fcntl(_pipe_sign[0], F_SETFD, FD_CLOEXEC);
                //::fcntl(_pipe_sign[1], F_SETFL, O_NONBLOCK); //写时需要阻塞
                ::fcntl(_pipe_sign[1], F_SETFD, FD_CLOEXEC);
                _sign_write = _pipe_sign[1];  // 1必须用于写
                _sign_read = _pipe_sign[0];   // 0用于读
            }
            ~sync_signal() {
                close(_sign_write);
                close(_sign_read);
            }

            inline void send(const int &num = 1) {
                vector<char> data;
                for (int i = 0; i < num; i++) {
                    data.push_back(char(1));
                }
                write(_sign_write, data.data(), num);
            }
            inline void wait(const int &num = 1) {
                vector<char> data(num);
                int readed_length = 0;
                struct timeval timeout;
                if (time_out_check) {
                    timeout.tv_sec = time_out_sec;
                }
                timeout.tv_usec = 10000;
                fd_set set;
                int rv;
                do {
                    FD_ZERO(&set);            /* clear the set */
                    FD_SET(_sign_read, &set); /* add our file descriptor to the set */

                    rv = select(_sign_read + 1, &set, NULL, NULL, &timeout);
                    if (rv == -1) {
                        hb_throw(hb_sync_exception().msg("sync_signal select read sign error!"));
                    } else if (rv == 0)
                        if (time_out_check) {
                            hb_throw(
                                hb_sync_exception().msg("sync_signal read sign time out error!"));
                        } else
                            continue;
                    else
                        readed_length += read(_sign_read, data.data(),
                                              num - readed_length); /* there was data to read */
                    if (readed_length >= num) break;
                } while (1);
            }

          private:
            //防止复制
            sync_signal(const sync_signal &&c){};
            sync_signal(const sync_signal &c){};
            void operator=(const sync_signal &c) {}
            void operator=(const sync_signal &&c) {}

          private:
            int _pipe_sign[2];
            int _sign_write;
            int _sign_read;
            int time_out_sec = 30;
            bool time_out_check = true;
        };

    }  // namespace sycn
}  // namespace hb

#endif  // CPPSERVER_SIGNAL_H
