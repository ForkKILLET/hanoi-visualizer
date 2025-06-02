#include "systems/disable.hpp"
#include "comps/disabled.hpp"

void DisableSystem::update() {
    for (auto entity : entities()) {
        auto dyn_disale = ecs.get_comp<DynamicDisabledComp>(entity);
        if (dyn_disale->is_disabled()) ecs.emplace_comp_overwrite<DisabledComp>(entity);
        else ecs.remove_comp_or_ignore<DisabledComp>(entity);
    }
}