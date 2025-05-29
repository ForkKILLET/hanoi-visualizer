#pragma once
#include <concepts>
#include "core/ecs.hpp"

class EntityBuilder {
public:
    EntityBuilder(ECS& ecs, Entity entity) :
        ecs(ecs), entity(entity) {}
    EntityBuilder(ECS& ecs) :
        EntityBuilder(ecs, ecs.create_entity()) {}

    virtual Entity build() {
        return entity;
    }

    template <typename B>
    requires std::derived_from<B, EntityBuilder>
    B use() {
        build();
        return B(ecs, entity);
    }

    template <typename B>
    requires std::derived_from<B, EntityBuilder>
    B& as() {
        return dynamic_cast<B&>(*this);
    }

protected:
    ECS& ecs;
    Entity entity;
};

inline EntityBuilder ECS::build_entity() {
    return EntityBuilder(*this);
}