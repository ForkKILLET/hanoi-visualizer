#pragma once
#include "core/ecs.hpp"
#include "comps/text.hpp"
#include "entities/pos.hpp"

class TextBuilder : public BoundBuilder {
public:
    using BoundBuilder::BoundBuilder;

    decltype(auto) text(std::string text) {
        text_.text = std::move(text);
        return *this;
    }

    decltype(auto) color(Color color) {
        text_.color = color;
        return *this;
    }

    decltype(auto) font_size(int font_size) {
        text_.font_size = font_size;
        return *this;
    }

    Entity build() override {
        ecs.emplace_comp<TextComp>(entity, text_);
        return BoundBuilder::build();
    }

protected:
    TextComp text_ {};
};
