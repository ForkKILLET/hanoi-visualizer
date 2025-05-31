#include <cassert>
#include <raylib.h>
#include "systems/hanoi.hpp"
#include "comps/hanoi.hpp"
#include "entities/hanoi_disk.hpp"
#include "comps/box.hpp"
#include "systems/animation.hpp"
#include "utils/hanoi.hpp"
#include "utils/timing_func.hpp"

void HanoiSystem::reset(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, DiskId disk_count) {
    hanoi->disk_count = disk_count;
    hanoi->is_playing = false;
    hanoi->solution = HanoiSolution::solve(disk_count);
    hanoi_bound->size = Size {
        HANOI_WIDTH,
        disk_count * DISK_HEIGHT + 1
    };

    for (auto $disk : hanoi->disks) {
        ecs.destroy_entity($disk);
    }
    hanoi->disks.clear();
    hanoi->disks.reserve(disk_count);
    hanoi->disks.resize(disk_count);

    for (auto& rod : hanoi->rods) {
        rod.clear();
        rod.reserve(disk_count);
    }

    float disk_width_step = ROD_WIDTH / disk_count;
    for (DiskId index = 0; index < disk_count; ++ index) {
        DiskId id = disk_count - index - 1;
        hanoi->rods[0].push_back(id);
        hanoi->disks[id] = ecs.build_entity()
            .use<HanoiDiskBuilder>()
                .id(id)
                .size({
                    disk_width_step * (id + 1),
                    DISK_HEIGHT + 1
                })
                .anchor(calc_disk_pos(hanoi, hanoi_bound, 0, index), TOP_CENTER)
            .build();
    }
}

void HanoiSystem::apply_step(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step) {
    auto& from_rod = hanoi->rods[step.from];
    auto& to_rod = hanoi->rods[step.to];

    assert(! from_rod.empty());
    DiskId disk_id = from_rod.back();

    auto $disk = hanoi->disks[disk_id];
    auto disk_anchor = ecs.get_comp<AnchorComp>($disk);
    auto new_pos = calc_disk_pos(hanoi, hanoi_bound, step.to, to_rod.size());

    ecs.emplace_comp_overwrite<AnimationComp>($disk, Animation {
        .delay = 0.0f,
        .duration = 0.5f,
        .mode = ANIMATION_MODE_ONCE,
        .track = AnimationTrack<Vector2> {
            .property = CompLens<Vector2>(ecs, &AnchorComp::pos),
            .from = disk_anchor->pos,
            .to = new_pos,
            .timing_func = TimingFuncs::ease_in_out,
        }
    });

    from_rod.pop_back();
    to_rod.push_back(disk_id);
}

void HanoiSystem::unapply_step(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step) {
    apply_step(hanoi, hanoi_bound, {step.to, step.from});
}

Vector2 HanoiSystem::calc_disk_pos(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, RodId rod_index, DiskId disk_index) {
    return hanoi_bound->pos + Vector2 {
        (rod_index + 0.5f) * ROD_WIDTH + rod_index * ROD_GAP,
        (hanoi->disk_count - disk_index - 1) * DISK_HEIGHT
    };
}

void HanoiSystem::update() {
    for (auto $hanoi : entities()) {
        auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
        if (hanoi->needs_reset) {
            auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);
            reset(hanoi, hanoi_bound, hanoi->disk_count);
            hanoi->needs_reset = false;
        }
    }
}

void HanoiSystem::step_next(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound) {
    if (hanoi->solution.has_next()) {
        auto step = hanoi->solution.current_step();
        apply_step(hanoi, hanoi_bound, step);
        hanoi->solution.next();
    }
}

void HanoiSystem::step_prev(HanoiCompPtr hanoi, BoundCompPtr hanoi_bound) {
    if (hanoi->solution.has_prev()) {
        hanoi->solution.prev();
        auto step = hanoi->solution.current_step();
        unapply_step(hanoi, hanoi_bound, step);
    }
}