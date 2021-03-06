#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

namespace hb {
    namespace time {
        using namespace std;

        inline uint64_t timestamp(const string& date_str) {
            boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
            boost::posix_time::time_duration time_from_epoch
                = boost::posix_time::time_from_string(date_str) - epoch;
            return time_from_epoch.total_seconds();
        }
        inline int64_t timestamp() {
            boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
            boost::posix_time::time_duration time_from_epoch
                = boost::posix_time::second_clock::local_time() - epoch;
            return time_from_epoch.total_seconds();
        }
        inline int64_t micro_timestamp() {
            boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
            boost::posix_time::time_duration time_from_epoch
                = boost::posix_time::microsec_clock::local_time() - epoch;
            return time_from_epoch.total_microseconds();
        }
        inline int day_of_week() {
            boost::posix_time::ptime tw(boost::posix_time::second_clock::local_time());
            int week = tw.date().day_of_week().as_number();  //.as_short_string()
            return week;
        }
        // sprintf(szLine, "%04d-%02d-%02d %02d:%02d:%02d:%03d", td.year(), td.month(),
        // td.day().as_number(), tt.hours(), tt.minutes(), tt.seconds(), ssss.total_microseconds() %
        // 1000); 所以minutes,seconds 不会超过60的
        inline int day_of_month() {
            boost::posix_time::ptime tw(boost::posix_time::second_clock::local_time());
            auto dt = tw.date().year_month_day();
            return dt.day;
        }
        inline int hours_of_day() {
            boost::posix_time::time_duration hms(
                boost::posix_time::second_clock::local_time().time_of_day());
            return hms.hours();
        }
        inline int minutes_of_day() {
            boost::posix_time::time_duration hms(
                boost::posix_time::second_clock::local_time().time_of_day());
            return hms.minutes();
        }
        inline int seconds_of_day() {
            boost::posix_time::time_duration hms(
                boost::posix_time::second_clock::local_time().time_of_day());
            return hms.seconds();
        }
    }  // namespace time
}  // namespace hb