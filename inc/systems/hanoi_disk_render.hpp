#pragma once
#include "core/ecs.hpp"
#include "comps/disabled.hpp"
#include "comps/hanoi_disk.hpp"
#include "comps/box.hpp"

class HanoiDiskRenderSystem : public System {
public:
    HanoiDiskRenderSystem(ECS& ecs) : System(ecs) {
        with_comp<BoundComp>();
        with_comp<HanoiDiskComp>();
        without_comp<DisabledComp>();
    }
    
    void update() override;
};