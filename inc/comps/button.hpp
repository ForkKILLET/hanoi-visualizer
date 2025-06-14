#pragma once
#include <optional>
#include <raylib.h>
#include "core/ecs.hpp"

struct ButtonComp : public Comp {
    ButtonComp() = default;

    bool is_highlighted { false };

    std::optional<Color> disabled_color {};
    std::optional<Color> normal_color {};
    std::optional<Color> hover_color {};
    std::optional<Color> active_color {};
    std::optional<Color> highlight_color {};
};
using ButtonCompPtr = std::shared_ptr<ButtonComp>;