#pragma once
#include "ecs.hpp"
#include "delegate.hpp"

struct ClickableComp : Comp {
    ClickableComp(Delegate<> on_click) :
        on_click(on_click) {}

    Delegate<> on_click {};
};
