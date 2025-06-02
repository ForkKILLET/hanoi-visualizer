#pragma once
#include "core/ecs.hpp"
#include "comps/box.hpp"
#include "comps/disabled.hpp"

class AnchorSystem : public System {
public:
    AnchorSystem(ECS& ecs) : System(ecs) {
        with_comp<AnchorComp>();
        with_comp<BoundComp>();
        without_comp<DisabledComp>();
    }

    void update() override;
};
