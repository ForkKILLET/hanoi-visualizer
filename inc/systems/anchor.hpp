#pragma once
#include "core/ecs.hpp"
#include "comps/transform.hpp"

class AnchorSystem : public System {
public:
    AnchorSystem(ECS& ecs) : System(ecs) {
        with_comp<AnchorComp>();
        with_comp<BoundComp>();
    }

    void update() override;
};
