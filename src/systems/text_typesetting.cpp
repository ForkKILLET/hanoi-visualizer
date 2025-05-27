#include <raylib.h>
#include "ecs.hpp"
#include "comps/text.hpp"
#include "comps/transform.hpp"
#include "systems/text_typesetting.hpp"

void TextTypesettingSystem::update() {
    for (auto entity : entities()) {
        auto text = ecs.get_comp<TextComp>(entity);
        auto bound = ecs.get_comp<BoundComp>(entity);

        bound->size = {
            static_cast<float>(MeasureText(text->text.c_str(), text->font_size)),
            static_cast<float>(text->font_size)
        };
    }
}