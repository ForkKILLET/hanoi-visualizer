#include "systems/text_render.hpp"
#include "comps/text.hpp"
#include "comps/transform.hpp"
#include "raylib.h"

void TextRenderSystem::update() {
    for (auto entity : entities()) {
        auto text = ecs.get_comp<TextComp>(entity);
        auto pos = ecs.get_comp<PosComp>(entity);

        DrawText(
            text->text.c_str(),
            pos->x,
            pos->y,
            text->font_size,
            text->color
        );
    }
}