#include "systems/clock_render.hpp"
#include <format>

void ClockRenderSystem::update() {
    for (auto $clock : entities()) {
        auto clock = ecs.get_comp<ClockComp>($clock);
        auto text = ecs.get_comp<TextComp>($clock);

        int minutes = static_cast<int>(clock->time_elapsed / 60);
        int seconds = static_cast<int>(clock->time_elapsed) % 60;
        int milliseconds = static_cast<int>(
            (clock->time_elapsed - static_cast<int>(clock->time_elapsed)) * 10
        );

        text->text = std::format("{:02}:{:02}.{}", minutes, seconds, milliseconds);
    }
}