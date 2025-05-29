#include <raylib.h>
#include "systems/timer.hpp"

void TimerSystem::update() {
    float time_delta = GetFrameTime();

    for (auto $timer : entities()) {
        auto timer = ecs.get_comp<TimerComp>($timer);
        if (! timer->is_running) continue;

        timer->time_elapsed += time_delta;

        if (timer->time_elapsed >= timer->time_limit) {
            timer->on_timeout();
            if (timer->is_infinite) {
                do timer->time_elapsed -= timer->time_limit;
                while (timer->time_elapsed >= timer->time_limit);
            }
            else timer->is_running = false;
        }
    }
}