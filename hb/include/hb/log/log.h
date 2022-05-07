//
// Created by 李卫东 on 2019-11-16.
//
#pragma once

#include <hb/error/exception.h>
#include<hb/util/util.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <mutex>

namespace hb::log {
    namespace logging = boost::log;
    namespace src = boost::log::sources;
    namespace sinks = boost::log::sinks;
    namespace keywords = boost::log::keywords;
    namespace trivial = boost::log::trivial;
    namespace bfs = boost::filesystem;
    namespace attrs = boost::log::attributes;

    extern std::mutex mx;

    void initialize_config(const bfs::path& file);
    // Convert file path to only the filename
    inline std::string path_to_filename(std::string path) {
        bfs::path file_path(path);
        return file_path.filename().string();
        // return path.substr(path.find_last_of("/\\")+1);
    }

    BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(general_logger,
                                        src::severity_logger_mt<trivial::severity_level>) {
        // Do something that needs to be done on logger initialization,
        // e.g. add a stop watch attribute.
        logging::add_common_attributes();
        logging::register_simple_formatter_factory<trivial::severity_level, char>("Severity");
        logging::register_simple_filter_factory<trivial::severity_level, char>("Severity");
        // register_udp_sink();
        src::severity_logger_mt<trivial::severity_level> lg(keywords::severity
                                                            = trivial::trace);
        lg.add_attribute("Channel", attrs::constant<std::string>("general"));
        lg.add_attribute("File", attrs::mutable_constant<std::string>(""));
        lg.add_attribute("Line", attrs::mutable_constant<int>(0));
        lg.add_attribute("Function", attrs::mutable_constant<std::string>(""));
        return lg;
    }
}  // namespace hb

#define SET_CUSTOM_ATTRIBUTE(VALUE_TYPE, KEY, VALUE)                                  \
    boost::log::attribute_cast<boost::log::attributes::mutable_constant<VALUE_TYPE>>( \
        hb::log::general_logger::get().get_attributes()[KEY])                         \
        .set(VALUE);

#define LOG_LOCATION(SERVERITY)                                                    \
    SET_CUSTOM_ATTRIBUTE(int, "Line", __LINE__)                                    \
    SET_CUSTOM_ATTRIBUTE(std::string, "File", hb::log::path_to_filename(__FILE__)) \
    SET_CUSTOM_ATTRIBUTE(std::string, "Function", __func__)                        \
    BOOST_LOG_SEV(hb::log::general_logger::get(), SERVERITY)

// 每次都复制一份新的log, 或者多线程锁
// #define SET_CUSTOM_ATTRIBUTE(LG, VALUE_TYPE, KEY, VALUE)                                  \
//     boost::log::attribute_cast<boost::log::attributes::mutable_constant<VALUE_TYPE>>( \
//         LG.get_attributes()[KEY])                         \
//         .set(VALUE);

// #define LOG_LOCATION(SERVERITY)                                                             \
//     ({                                                                                      \
//         auto lg = hb::log::general_logger::get();                                           \
//         SET_CUSTOM_ATTRIBUTE(lg, int, "Line", __LINE__);                                    \
//         SET_CUSTOM_ATTRIBUTE(lg, std::string, "File", hb::log::path_to_filename(__FILE__)); \
//         SET_CUSTOM_ATTRIBUTE(lg, std::string, "Function", __func__);                        \
//         BOOST_LOG_SEV(lg, SERVERITY)                                                       \
//     })                                                                                      \

#define _log_trace LOG_LOCATION(hb::log::trivial::trace)
#define _log_debug LOG_LOCATION(hb::log::trivial::debug)
#define _log_info LOG_LOCATION(hb::log::trivial::info)
#define _log_warn LOG_LOCATION(hb::log::trivial::warning)
#define _log_error LOG_LOCATION(hb::log::trivial::error)
#define _log_fatal LOG_LOCATION(hb::log::trivial::fatal)

// old code
// #define LOG_TRACE(fmt, ...)                        \
//     do {                                           \
//         char str[log_max_length];                  \
//         sprintf(str, "%s" fmt, "", ##__VA_ARGS__); \
//         log_trace << str;                          \
//     } while (0)

#ifdef DEBUG
#define LOG_TRACE(fmt, ...)                                                 \
    do {                                                                    \
        auto str = hb::util::format_args(fmt, ##__VA_ARGS__);               \
        std::unique_lock<std::mutex> lock(hb::log::mx);                     \
        _log_trace << str;                                                  \
    } while (0)
#else
#define LOG_TRACE(fmt, ...) \

#endif

#define log_trace LOG_TRACE

#ifdef DEBUG
#define LOG_DEBUG(fmt, ...)                                                 \
    do {                                                                    \
        auto str = hb::util::format_args(fmt, ##__VA_ARGS__);               \
        std::unique_lock<std::mutex> lock(hb::log::mx);                     \
        _log_debug << str;                                                  \
    } while (0)
#else
#define LOG_DEBUG(fmt, ...) \

#endif

#define log_debug LOG_DEBUG

#define LOG_INFO(fmt, ...)                                                  \
    do {                                                                    \
        auto str = hb::util::format_args(fmt, ##__VA_ARGS__);               \
        std::unique_lock<std::mutex> lock(hb::log::mx);                     \
        _log_info << str;                                                   \
    } while (0)

#define log_info LOG_INFO

#define LOG_WARN(fmt, ...)                                                  \
    do {                                                                    \
        auto str = hb::util::format_args(fmt, ##__VA_ARGS__);               \
        std::unique_lock<std::mutex> lock(hb::log::mx);                     \
        _log_warn << str;                                                   \
    } while (0)

#define log_warn LOG_WARN

#define LOG_ERROR(fmt, ...)                                                 \
    do {                                                                    \
        auto str = hb::util::format_args(fmt, ##__VA_ARGS__);               \
        std::unique_lock<std::mutex> lock(hb::log::mx);                     \
        _log_error << str;                                                  \
    } while (0)

#define log_error LOG_ERROR

#define LOG_FATAL(fmt, ...)                                                 \
    do {                                                                    \
        auto str = hb::util::format_args(fmt, ##__VA_ARGS__);               \
        std::unique_lock<std::mutex> lock(hb::log::mx);                     \
        _log_fatal << str;                                                  \
    } while (0)

#define log_fatal LOG_FATAL

#define LOG_THROW(sum, e)                            \
    ({                                               \
        std::string error_str = exception_data(e);   \
        std::unique_lock<std::mutex> lock(hb::log::mx);     \
        _log_fatal << sum << " detail:" << error_str; \
        error_str;                                   \
    })

#define log_throw LOG_THROW
