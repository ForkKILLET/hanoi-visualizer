#include "core/ecs.hpp"
#include "comps/box.hpp"
#include "builder.hpp"

class PaddingBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) padding_left(float left) {
        padding_.left = left;
        return *this;
    }
    decltype(auto) padding_top(float top) {
        padding_.top = top;
        return *this;
    }
    decltype(auto) padding_right(float right) {
        padding_.right = right;
        return *this;
    }
    decltype(auto) padding_bottom(float bottom) {
        padding_.bottom = bottom;
        return *this;
    }

    decltype(auto) padding_horizontal(float horizontal) {
        padding_.left = horizontal;
        padding_.right = horizontal;
        return *this;
    }
    decltype(auto) padding_vertical(float vertical) {
        padding_.top = vertical;
        padding_.bottom = vertical;
        return *this;
    }

    decltype(auto) padding_all(float left, float top, float right, float bottom) {
        padding_.left = left;
        padding_.top = top;
        padding_.right = right;
        padding_.bottom = bottom;
        return *this;
    }
    decltype(auto) padding_all(float padding) {
        padding_.left = padding_.top = padding_.right = padding_.bottom = padding;
        return *this;
    }
    decltype(auto) padding_all(float horizontal, float vertical) {
        padding_.left = padding_.right = horizontal;
        padding_.top = padding_.bottom = vertical;
        return *this;
    }

    Entity build() override {
        ecs.emplace_comp<PaddingComp>(entity, padding_);
        return EntityBuilder::build();
    }

protected:
    PaddingComp padding_ {};
};