#pragma once
#include "core/ecs.hpp"
#include "comps/hanoi_disk.hpp"
#include "comps/transform.hpp"

class HanoiDiskRenderSystem : public System {
public:
    HanoiDiskRenderSystem(ECS& ecs) : System(ecs) {
        with_comp<BoundComp>();
        with_comp<HanoiDiskComp>();
    }
    
    void update() override;
};