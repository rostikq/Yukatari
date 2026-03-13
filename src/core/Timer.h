//
// Created by user on 3/13/2026.
//

#ifndef YUKATARI_TIMER_H
#define YUKATARI_TIMER_H

#include  <chrono>

class Timer {
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
public:
    ~Timer() = default;

    void reset() {
        startTime = std::chrono::steady_clock::now();
    }
    float getPassedTimeInSeconds() const {
        return std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count();
    }
};


#endif //YUKATARI_TIMER_H