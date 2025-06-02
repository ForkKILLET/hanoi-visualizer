#pragma once
#include "entities/builder.hpp"
#include "utils/functional.hpp"
#include "comps/disabled.hpp"

class DisableBuilder : public virtual EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) disable_when(Condition is_disabled) {
        ecs.emplace_comp<DynamicDisabledComp>(entity, is_disabled);
        return *this;
    }

    decltype(auto) enable_when(Condition is_enabled) {
        return disable_when(f_not(is_enabled));
    }
};