#pragma once
#include <optional>
#include <raylib.h>
#include "core/ecs.hpp"
#include "utils/delegate.hpp"

struct ClickableComp : Comp {
    ClickableComp() = default;
    ClickableComp(Delegate<Entity> on_click) :
        on_click(on_click) {}

    Delegate<Entity> on_click {};
    Delegate<Entity> on_mouse_down {};
    Delegate<Entity> on_mouse_up {};
    Delegate<Entity> on_mouse_enter {};
    Delegate<Entity> on_mouse_leave {};

    bool is_hovering { false };
    bool is_active { false };
    bool is_disabled { false };
    std::optional<Vector2> mouse_down_pos {};
};
using ClickableCompPtr = std::shared_ptr<ClickableComp>;