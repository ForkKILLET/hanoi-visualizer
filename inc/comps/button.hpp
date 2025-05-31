#pragma once
#include <optional>
#include "core/ecs.hpp"
#include "raylib.h"

struct ButtonComp : public Comp {
    ButtonComp() = default;

    bool is_disabled { false };
    
    std::optional<Color> disabled_color {};
    std::optional<Color> normal_color {};
    std::optional<Color> hover_color {};
    std::optional<Color> active_color {};
};
using ButtonCompPtr = std::shared_ptr<ButtonComp>;