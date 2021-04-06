#include "mytime.h"

Time::~Time() = default;

void Time::start() {
    begin = std::chrono::steady_clock::now();
}

void Time::end() {
    finish = std::chrono::steady_clock::now();
    interval = std::chrono::duration<double>(finish - begin).count();
}

double Time::get_time_used() const {
    return interval;
}
