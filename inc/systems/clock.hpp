#pragma once
#include "core/ecs.hpp"
#include "comps/clock.hpp"
#include "comps/disabled.hpp"

class ClockSystem : public System {
public:
    ClockSystem(ECS& ecs) : System(ecs) {
        with_comp<ClockComp>();
        without_comp<DisabledComp>();
    }

    void update() override;
};