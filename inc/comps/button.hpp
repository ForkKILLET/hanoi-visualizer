#pragma once
#include <optional>
#include "core/ecs.hpp"
#include "raylib.h"

struct ButtonStyleComp : public Comp {
    ButtonStyleComp() = default;

    bool is_inited { false };
    
    std::optional<Color> normal_color {};
    std::optional<Color> hover_color {};
    std::optional<Color> active_color {};
};