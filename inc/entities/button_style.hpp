#pragma once
#include "comps/button.hpp"
#include "builder.hpp"

class ButtonBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) normal_color(Color color) {
        button_.normal_color = color;
    }
    decltype(auto) disabled_color(Color color) {
        button_.disabled_color = color;
    }
    decltype(auto) hover_color(Color color) {
        button_.hover_color = color;
    }
    decltype(auto) active_color(Color color) {
        button_.active_color = color;
    }

    Entity build() override {
        ecs.emplace_comp<ButtonComp>(entity, button_);
        return entity;
    }

protected:
    ButtonComp button_;
};