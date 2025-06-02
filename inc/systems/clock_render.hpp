#pragma once

#include "comps/clock.hpp"
#include "comps/disabled.hpp"
#include "comps/text.hpp"
class ClockRenderSystem : public System {
public:
    ClockRenderSystem(ECS& ecs) : System(ecs) {
        with_comp<ClockComp>();
        with_comp<TextComp>();
        without_comp<DisabledComp>();
    }

    void update() override;
};