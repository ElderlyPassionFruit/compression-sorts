#include "compression_sorts/time.hpp"

#include <chrono>

namespace CompressionSorts {

TimePoint Now() {
    return std::chrono::high_resolution_clock::now();
}

Time MesuareTime(std::function<void()> routine) {
    TimePoint start_time = Now();
    routine();
    TimePoint finish_time = Now();
    return std::chrono::duration_cast<Time>(finish_time - start_time);
}

TimeBudget::TimeBudget(Time budget) : budget_(budget), start_time_(Now()) {
}

Time TimeBudget::GetElapsedTime() const {
    TimePoint current_time = Now();
    return std::chrono::duration_cast<Time>(current_time - start_time_);
}

bool TimeBudget::operator()() {
    return GetElapsedTime() < budget_;
}

}  // namespace CompressionSorts
