#pragma once
#include "core/ecs.hpp"
#include "utils/delegate.hpp"

struct ClickableComp : Comp {
    ClickableComp(Delegate<> on_click) :
        on_click(on_click) {}

    Delegate<> on_click {};
};
using ClickableCompPtr = std::shared_ptr<ClickableComp>;