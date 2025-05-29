#pragma once
#include <string>
#include "core/ecs.hpp"
#include "utils/hanoi.hpp"

struct HanoiDiskComp : public Comp {
    HanoiDiskComp(DiskId id) :
        id(id),
        id_str(std::to_string(id + 1))
    {}

    DiskId id;

    std::string id_str;
};
using HanoiDiskCompPtr = std::shared_ptr<HanoiDiskComp>;