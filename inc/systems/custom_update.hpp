#pragma once
#include "core/ecs.hpp"
#include "comps/updater.hpp"

class CustomUpdateSystem : public System {
public:
    CustomUpdateSystem(ECS& ecs) : System(ecs) {
        with_comp<UpdaterComp>();
    }

    void update() override;
};