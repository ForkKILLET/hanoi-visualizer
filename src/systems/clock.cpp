#include "systems/clock.hpp"

void ClockSystem::update() {
    for (auto $clock : entities()) {
        auto clock = ecs.get_comp<ClockComp>($clock);
        if (clock->is_paused) continue;

        clock->time_elapsed += GetFrameTime();
    }
}