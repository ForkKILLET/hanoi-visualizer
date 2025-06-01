#pragma once
#include "core/ecs.hpp"
#include "comps/clickable.hpp"
#include "comps/box.hpp"

class ClickSystem : public System {
public:
    ClickSystem(ECS& ecs) : System(ecs) {
        with_comp<ClickableComp>();
        with_comp<BoundComp>();
    }

    void update() override;

    static constexpr float MAX_CLICK_DISTANCE = 4.0f;
};