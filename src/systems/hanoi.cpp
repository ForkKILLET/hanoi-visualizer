#include <cassert>
#include <optional>
#include <set>
#include <raylib.h>
#include "core/ecs.hpp"
#include "systems/hanoi.hpp"
#include "comps/clickable.hpp"
#include "comps/clock.hpp"
#include "comps/hanoi.hpp"
#include "entities/clickable.hpp"
#include "entities/updater.hpp"
#include "entities/hanoi_disk.hpp"
#include "comps/box.hpp"
#include "comps/hanoi_disk.hpp"
#include "systems/animation.hpp"
#include "utils/hanoi.hpp"
#include "utils/timing_func.hpp"

void HanoiSystem::reset(
    HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, DiskId disk_count, ClockCompPtr user_clock
) {
    clear_phantom_disks(hanoi);
    hanoi->is_solved = false;
    hanoi->selected_disk = std::nullopt;
    hanoi->user_step_count = 0;

    hanoi->disk_count = disk_count;
    hanoi->solution = HanoiSolution::solve(disk_count);
    hanoi_bound->size = Size {
        HANOI_WIDTH,
        disk_count * DISK_HEIGHT + 1
    };

    for (auto $disk : hanoi->$disks) ecs.destroy_entity($disk);
    hanoi->$disks.clear();
    hanoi->$disks.reserve(disk_count);
    hanoi->$disks.resize(disk_count);

    for (auto& rod : hanoi->rods) {
        rod.clear();
        rod.reserve(disk_count);
    }

    float disk_width_step = ROD_WIDTH / disk_count;
    for (DiskId index = 0; index < disk_count; ++ index) {
        DiskId id = disk_count - index - 1;
        hanoi->rods[0].push_back(id);
        hanoi->$disks[id] = ecs.build_entity()
            .use<HanoiDiskBuilder>()
                .id(id)
                .size({
                    disk_width_step * (id + 1),
                    DISK_HEIGHT + 1
                })
                .anchor(calc_disk_pos(hanoi, hanoi_bound, 0, index), TOP_CENTER)
            .use<ClickableBuilder>()
                .on_click([&, hanoi, hanoi_bound, user_clock](Entity $self) {
                    auto disk = ecs.get_comp<HanoiDiskComp>($self);
                    toggle_disk_select(hanoi, hanoi_bound, disk, user_clock);
                })
            .build();
    }
}

void HanoiSystem::apply_step(
    HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step
) {
    auto& rod_from = hanoi->rods[step.from];
    auto& rod_to = hanoi->rods[step.to];

    assert(! rod_from.empty());
    DiskId disk_id = rod_from.back();

    auto $disk = hanoi->$disks[disk_id];
    auto disk_anchor = ecs.get_comp<AnchorComp>($disk);
    auto new_pos = calc_disk_pos(hanoi, hanoi_bound, step.to, rod_to.size());

    auto disk = ecs.get_comp<HanoiDiskComp>($disk);
    disk->is_highlighted = true;
    ecs.emplace_comp_overwrite<AnimationComp>($disk, Animation {
        .delay = 0.0f,
        .duration = 0.5f,
        .mode = ANIMATION_MODE_ONCE,
        .track = AnimationTrack<Vector2> {
            .property = CompLens<Vector2>(ecs, &AnchorComp::pos),
            .from = disk_anchor->pos,
            .to = new_pos,
            .timing_func = TimingFuncs::ease_in_out,
        },
        .on_finish = {
            [&, disk] { disk->is_highlighted = false; }
        }
    });

    rod_from.pop_back();
    rod_to.push_back(disk_id);
}

void HanoiSystem::unapply_step(
    HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, const HanoiSolution::Step& step
) {
    apply_step(hanoi, hanoi_bound, { step.to, step.from });
}

Vector2 HanoiSystem::calc_disk_pos(
    HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, RodId rod_index, DiskId disk_index
) const {
    return hanoi_bound->pos + Vector2 {
        (rod_index + 0.5f) * ROD_WIDTH + rod_index * ROD_GAP,
        (hanoi->disk_count - disk_index - 1) * DISK_HEIGHT
    };
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

void HanoiSystem::clear_phantom_disks(HanoiCompPtr hanoi) {
    for (auto $phantom_disk : hanoi->$phantom_disks) ecs.destroy_entity($phantom_disk);
    hanoi->$phantom_disks.clear();
}

void HanoiSystem::toggle_disk_select(
    HanoiCompPtr hanoi, BoundCompPtr hanoi_bound, HanoiDiskCompPtr disk, ClockCompPtr user_clock
) {
    assert(hanoi->is_user_mode);

    clear_phantom_disks(hanoi);

    if (hanoi->selected_disk && *hanoi->selected_disk == disk->id) {
        disk->is_highlighted = false;
        hanoi->selected_disk = std::nullopt;
    }
    else {
        if (hanoi->selected_disk) {
            auto $selected_disk = hanoi->$disks[*hanoi->selected_disk];
            auto selected_disk = ecs.get_comp<HanoiDiskComp>($selected_disk);
            selected_disk->is_highlighted = false;
        }
        disk->is_highlighted = true;
        hanoi->selected_disk = disk->id;

        RodId from;
        for (RodId rod_index = 0; rod_index < 3; ++ rod_index) {
            const auto& rod = hanoi->rods[rod_index];
            if (! rod.empty() && rod.back() == disk->id) {
                from = rod_index;
                break;
            }
        }

        for (RodId rod_index = 0; rod_index < 3; ++ rod_index) {
            const auto& rod = hanoi->rods[rod_index];
            if (rod.empty() || rod.back() > disk->id) {
                Entity $phantom_disk = ecs.build_entity()
                    .use<HanoiDiskBuilder>()
                        .is_phantom()
                        .size({
                            ROD_WIDTH / hanoi->disk_count * (disk->id + 1),
                            DISK_HEIGHT + 1
                        })
                        .anchor(
                            calc_disk_pos(hanoi, hanoi_bound, rod_index, rod.size()),
                            TOP_CENTER
                        )
                    .use<ClickableBuilder>()
                        .on_click([&, hanoi, hanoi_bound, disk, from, to = rod_index, user_clock] {
                            ++ hanoi->user_step_count;
                            apply_step(hanoi, hanoi_bound, { from, to });
                            toggle_disk_select(hanoi, hanoi_bound, disk, user_clock);
                            if (hanoi->check_solved()) {
                                user_clock->is_paused = true;
                                hanoi->is_solved = true;
                                hanoi->on_user_solve();
                            }
                        })
                    .build();
                hanoi->$phantom_disks.push_back($phantom_disk);
            }
        }
    }
}

void HanoiSystem::update() {
    for (auto $hanoi : entities()) {
        auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
        if (hanoi->needs_reset) {
            auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);
            reset(hanoi, hanoi_bound, hanoi->disk_count, nullptr);
            hanoi->needs_reset = false;
        }

        bool is_user_mode = hanoi->is_user_mode;
        bool is_playing = hanoi->is_playing;
        bool is_solved = hanoi->is_solved;

        std::set<DiskId> clickable_disk_ids {};
        if (is_user_mode) {
            for (const auto& rod : hanoi->rods) {
                if (rod.empty()) continue;
                clickable_disk_ids.insert(rod.back());
            }
        }

        for (auto $disk : hanoi->$disks) {
            auto clickable = ecs.get_comp<ClickableComp>($disk);
            auto disk = ecs.get_comp<HanoiDiskComp>($disk);
            clickable->is_disabled = (
                is_solved ||
                ! is_user_mode ||
                ! is_playing ||
                ! clickable_disk_ids.contains(disk->id)
            );
        }
    }
}

