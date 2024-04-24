#pragma once

#include <chrono>
#include <functional>

namespace CompressionSorts {

using Time = std::chrono::nanoseconds;
using TimePoint = std::chrono::time_point<std::chrono::system_clock,
                                          std::chrono::duration<long, std::ratio<1, 1000000000>>>;

TimePoint Now();

Time MesuareTime(std::function<void()> /*routine*/);

class TimeBudget {
public:
    explicit TimeBudget(Time budget);

    Time GetElapsedTime() const;

    bool operator()();

private:
    const Time budget_;
    const TimePoint start_time_;
};

}  // namespace CompressionSorts
