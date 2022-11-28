#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * Stolen from internet and then edited:
 *      https://techoverflow.net/2018/03/30/iso8601-utc-time-as-stdstring-using-c11-chrono/
 */
inline std::string isoDateHoursFromNow(int hours)
{
    auto now = std::chrono::system_clock::now();
    auto then = now + std::chrono::hours(hours);
    auto itt = std::chrono::system_clock::to_time_t(then);
    std::ostringstream ss;
    ss << std::put_time(gmtime(&itt), "%FT%TZ");
    return ss.str();
}

#endif
