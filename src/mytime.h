#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time {
private:
    std::chrono::steady_clock::time_point begin, finish;
    double interval = 0.0;

public:
    Time() {
    };

    ~Time();

    void start();

    void end();

    double get_time_used() const;
};


#endif //TIME_H
