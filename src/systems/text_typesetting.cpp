#include <raylib.h>
#include "core/ecs.hpp"
#include "comps/text.hpp"
#include "comps/box.hpp"
#include "systems/text_typesetting.hpp"

void TextTypesettingSystem::update() {
    for (auto $text : entities()) {
        auto text = ecs.get_comp<TextComp>($text);
        auto bound = ecs.get_comp<BoundComp>($text);

        bound->size = {
            static_cast<float>(MeasureText(text->text.c_str(), text->font_size)),
            static_cast<float>(text->font_size)
        };
    }
}