#pragma once
#include "core/ecs.hpp"
#include "comps/disabled.hpp"

class DisableSystem : public System {
public:
    DisableSystem(ECS& ecs) : System(ecs) {
        with_comp<DynamicDisabledComp>();
    }

    void update() override;
};