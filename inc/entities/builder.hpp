#pragma once
#include "core/ecs.hpp"

class EntityBuilder {
public:
    EntityBuilder(ECS& ecs) :
        ecs(ecs), entity(ecs.create_entity()) {}

    virtual Entity build() { return entity; }

protected:
    ECS& ecs;
    Entity entity;
};