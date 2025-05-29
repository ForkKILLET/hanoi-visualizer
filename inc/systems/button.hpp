#pragma once
#include "core/ecs.hpp"
#include "comps/box.hpp"
#include "comps/button.hpp"

class ButtonSystem : public System {
public:
    ButtonSystem(ECS& ecs) : System(ecs) {
        with_comp<BorderComp>();
        with_comp<ButtonStyleComp>();
    }

    void update() override;
};