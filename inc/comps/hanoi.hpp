#pragma once
#include <raylib.h>
#include <vector>
#include "core/ecs.hpp"
#include "utils/hanoi.hpp"

struct HanoiComp : public Comp {
public:
    HanoiComp() = default;

    std::array<std::vector<DiskId>, 3> rods;
    DiskId disk_count;
    bool needs_reset { true };

    std::vector<Entity> disks;
    HanoiSolution solution;

    bool is_playing { false };
};
using HanoiCompPtr = std::shared_ptr<HanoiComp>;
