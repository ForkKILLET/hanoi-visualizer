#pragma once
#include "core/ecs.hpp"
#include "utils/delegate.hpp"

struct UpdaterComp : public Comp {
    UpdaterComp() = default;
    UpdaterComp(Delegate<Entity> updater) : updater(updater) {}

    Delegate<Entity> updater {};
};