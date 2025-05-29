#pragma once
#include "core/ecs.hpp"
#include "comps/text.hpp"
#include "comps/box.hpp"

class TextTypesettingSystem : public System {
public:
    TextTypesettingSystem(ECS& ecs) : System(ecs) {
        with_comp<TextComp>();
        with_comp<BoundComp>();
    }

    void update() override;
};