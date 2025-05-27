#pragma once
#include <format>
#include <raylib.h>
#include "builder.hpp"
#include "comps/transform.hpp"
#include "ecs.hpp"

class PosBuilder : public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) pos(Vector2 position) {
        define_pos();
        ecs.emplace_comp<PosComp>(entity, position);
        return *this;
    }

    decltype(auto) anchor(Vector2 pos, Vector2 anchor) {
        define_pos();
        ecs.emplace_comp<AnchorComp>(entity, pos, anchor);
        return *this;
    }

    Entity build() override {
        if (! pos_defined)
            throw std::runtime_error(std::format("Position not defined for entity {}.", entity));
        return EntityBuilder::build();
    }

protected:
    bool pos_defined = false;

    void define_pos() {
        if (pos_defined)
            throw std::runtime_error(std::format("Position already defined for entity {}.", entity));
        pos_defined = true;
    }
};