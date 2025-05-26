#include "timer.hpp"

void Timer::update() {
    time_elapsed_ += GetFrameTime();
    if (time_elapsed_ >= duration_) {
        action_();
        time_elapsed_ -= duration_;
    }
}