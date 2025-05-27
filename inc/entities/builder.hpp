#pragma once
#include "ecs.hpp"

class EntityBuilder {
public:
    EntityBuilder(ECS& ecs, Entity entity)
        : ecs(ecs), entity(entity) {}

    virtual Entity build() { return entity; }

protected:
    ECS& ecs;
    Entity entity;
};