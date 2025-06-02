#pragma once

#include "core/ecs.hpp"
#include "comps/clock.hpp"
#include "entities/builder.hpp"

class ClockBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    Entity build() {
        EntityBuilder::build();
        ecs.emplace_comp<ClockComp>(entity);
        return entity;
    }
};