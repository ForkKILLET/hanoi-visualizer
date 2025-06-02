#pragma once
#include <raylib.h>
#include <optional>
#include <vector>
#include "core/ecs.hpp"
#include "utils/delegate.hpp"
#include "utils/hanoi.hpp"

struct HanoiComp : public Comp {
public:
    HanoiComp() = default;

    bool check_solved() const {
        return rods[2].size() == disk_count;
    }

    std::array<std::vector<DiskId>, 3> rods;
    DiskId disk_count;
    bool needs_reset { true };

    std::vector<Entity> $disks;
    HanoiSolution solution;

    bool is_user_mode {};
    bool is_playing {};
    bool is_solved {};

    Delegate<> on_user_solve {};

    std::optional<DiskId> selected_disk {};
    std::vector<Entity> $phantom_disks {};
    unsigned int user_step_count {};
};
using HanoiCompPtr = std::shared_ptr<HanoiComp>;
