#include <raylib.h>
#include "systems/text_render.hpp"
#include "comps/text.hpp"
#include "comps/box.hpp"

void TextRenderSystem::update() {
    for (auto $text : entities()) {
        auto text = ecs.get_comp<TextComp>($text);
        auto bound = ecs.get_comp<BoundComp>($text);

        DrawText(
            text->text.c_str(),
            bound->pos.x,
            bound->pos.y,
            text->font_size,
            text->color
        );
    }
}