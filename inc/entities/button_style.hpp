#pragma once
#include "comps/button.hpp"
#include "builder.hpp"

class ButtonStyleBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) normal_color(Color color) {
        button_.normal_color = color;
    }
    decltype(auto) hover_color(Color color) {
        button_.hover_color = color;
    }
    decltype(auto) active_color(Color color) {
        button_.active_color = color;
    }

    Entity build() override {
        ecs.emplace_comp<ButtonStyleComp>(entity, button_);
        return entity;
    }

protected:
    ButtonStyleComp button_;
};