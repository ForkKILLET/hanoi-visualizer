#pragma once
#include <optional>
#include "core/ecs.hpp"
#include "core/vector.hpp"
#include "utils/delegate.hpp"

struct ClickableComp : Comp {
    ClickableComp() = default;
    ClickableComp(Delegate<> on_click) :
        on_click(on_click) {}

    Delegate<> on_click {};
    Delegate<> on_mouse_down {};
    Delegate<> on_mouse_up {};
    Delegate<> on_mouse_enter {};
    Delegate<> on_mouse_leave {};

    bool is_hovering { false };
    bool is_active { false };
    std::optional<VV2> mouse_down_pos {};
};
using ClickableCompPtr = std::shared_ptr<ClickableComp>;