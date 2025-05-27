#include <iostream>
#include <raylib.h>
#include "systems/text_render.hpp"
#include "comps/text.hpp"
#include "comps/transform.hpp"
#include "utils.hpp"

void TextRenderSystem::update() {
    for (auto entity : entities()) {
        auto text = ecs.get_comp<TextComp>(entity);
        auto bound = ecs.get_comp<BoundComp>(entity);

        DrawText(
            text->text.c_str(),
            bound->pos.x,
            bound->pos.y,
            text->font_size,
            text->color
        );
    }
}