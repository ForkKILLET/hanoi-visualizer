#pragma once
#include <raylib.h>
#include "core/ecs.hpp"
#include "comps/transform.hpp"
#include "comps/hanoi.hpp"
#include "utils/hanoi.hpp"

class HanoiSystem : public System {
public:
    HanoiSystem(ECS& ecs) : System(ecs) {
        with_comp<HanoiComp>();
        with_comp<BoundComp>();
    }

    void reset(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, DiskId disk_count);

    void apply_step(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step);
    void unapply_step(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step);
    void step(HanoiCompPtr hanoi, BoundCompPtr bound);

    Vector2 calc_disk_pos(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, RodId rod_index, DiskId disk_index);

    void update() override;
};