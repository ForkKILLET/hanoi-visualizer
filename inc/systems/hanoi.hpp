#pragma once
#include <raylib.h>
#include "core/ecs.hpp"
#include "comps/box.hpp"
#include "comps/clock.hpp"
#include "comps/disabled.hpp"
#include "comps/hanoi.hpp"
#include "comps/hanoi_disk.hpp"
#include "utils/hanoi.hpp"

class HanoiSystem : public System {
public:
    HanoiSystem(ECS& ecs) : System(ecs) {
        with_comp<HanoiComp>();
        with_comp<BoundComp>();
        without_comp<DisabledComp>();
    }

    void reset(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, DiskId disk_count, ClockCompPtr user_clock);

    void apply_step(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step);
    void unapply_step(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step);
    void step_next(HanoiCompPtr hanoi, BoundCompPtr bound);
    void step_prev(HanoiCompPtr hanoi, BoundCompPtr bound);

    Vector2 calc_disk_pos(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, RodId rod_index, DiskId disk_index) const;

    void clear_phantom_disks(HanoiCompPtr hanoi);
    void toggle_disk_select(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, HanoiDiskCompPtr disk, ClockCompPtr user_clock);

    void update() override;
};