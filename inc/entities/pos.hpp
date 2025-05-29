#pragma once
#include <format>
#include <raylib.h>
#include "builder.hpp"
#include "comps/box.hpp"
#include "core/ecs.hpp"

class BoundBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) pos(Vector2 pos) {
        define_pos();
        bound_.pos = pos;
        return *this;
    }

    decltype(auto) size(Size size) {
        bound_.size = size;
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
        ecs.emplace_comp<BoundComp>(entity, bound_);
        return EntityBuilder::build();
    }

protected:
    bool pos_defined = false;

    BoundComp bound_ {};

    void define_pos() {
        if (pos_defined)
            throw std::runtime_error(std::format("Position already defined for entity {}.", entity));
        pos_defined = true;
    }
};