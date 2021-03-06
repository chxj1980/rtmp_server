/*
MIT License

Copyright (c) 2016 ME_Kun_Han

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

The log system for rtmp server
*/

#ifndef RS_MODULE_LOG_H_
#define RS_MODULE_LOG_H_

#include <unistd.h>
#include "rs_common.h"
#include "rs_kernel_io.h"

namespace rs_log {

    class IRsLog {
    private:
        int32_t pid;
    public:
        IRsLog() {
            pid = getpid();
        };

        virtual ~IRsLog() = default;

    public:
        virtual void log(int64_t cid, std::string level, std::string message) = 0;

        int32_t get_pid() { return pid; };
    };

    class RsConsoleLog : public IRsLog {
    public:
        RsConsoleLog() = default;

        ~RsConsoleLog() override = default;

    public:
        void log(int64_t cid, std::string level, std::string message) override;
    };

    class RsLogManager {
    private:
        std::shared_ptr<char> msg;
        std::shared_ptr<IRsLog> log_interface;
    public:
        RsLogManager();

        RsLogManager(RsLogManager const &) = delete;

        RsLogManager(RsLogManager &&) = delete;

        RsLogManager &operator=(RsLogManager const &) = delete;

        virtual ~RsLogManager() = default;

    public:

        void change_log_interface(IRsLog *inter) {
            log_interface.reset(inter);
        }

        void info(IRsIO *io, const char *fmt, ...);

        void verbose(IRsIO *io, const char *fmt, ...);

        void trace(IRsIO *io, const char *fmt, ...);

        void warn(IRsIO *io, const char *fmt, ...);

        void error(IRsIO *io, const char *fmt, ...);

    public:
        virtual void log(int64_t cid, std::string level, std::string message) {
            if (log_interface == nullptr) {
                printf("%s\n", message.c_str());
            } else {
                log_interface->log(cid, level, message);
            }
        };

    public:
        static std::shared_ptr<RsLogManager> get_instance() {
            static std::shared_ptr<RsLogManager> ins = std::shared_ptr<RsLogManager>(
                    new RsLogManager());
            return ins;
        }
    };

}

#define rs_info(io, fmt, ...) rs_log::RsLogManager::get_instance()->info(io, fmt, ##__VA_ARGS__)
#define rs_verbose(io, fmt, ...) rs_log::RSLogManager::get_instance()->verbose(io, fmt, ##__VA_ARGS__)
#define rs_trace(io, fmt, ...) rs_log::RSLogManager::get_instance()->trace(io, fmt, ##__VA_ARGS__)
#define rs_warn(io, fmt, ...) rs_log::RsLogManager::get_instance()->warn(io, fmt, ##__VA_ARGS__)
#define rs_error(io, fmt, ...) rs_log::RsLogManager::get_instance()->error(io, fmt, ##__VA_ARGS__)

// TODO:FIXME: implement the log using disk.

#endif
