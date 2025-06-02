#pragma once
#include "core/ecs.hpp"
#include "comps/clickable.hpp"
#include "comps/box.hpp"
#include "comps/disabled.hpp"

class ClickSystem : public System {
public:
    ClickSystem(ECS& ecs) : System(ecs) {
        with_comp<ClickableComp>();
        with_comp<BoundComp>();
        without_comp<DisabledComp>();
    }

    void update() override;

    static constexpr float MAX_CLICK_DISTANCE = 4.0f;
};