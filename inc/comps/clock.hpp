#pragma once
#include <raylib.h>
#include "core/ecs.hpp"

struct ClockComp : public Comp {
    ClockComp() = default;

    float time_elapsed {};

    bool is_paused { false };
};
using ClockCompPtr = std::shared_ptr<ClockComp>;