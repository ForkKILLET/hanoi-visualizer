#pragma once
#include "core/ecs.hpp"
#include "comps/clickable.hpp"
#include "comps/transform.hpp"

class ClickSystem : public System {
public:
    ClickSystem(ECS& ecs) : System(ecs) {
        with_comp<ClickableComp>();
        with_comp<BoundComp>();
    }

    void update() override;
};