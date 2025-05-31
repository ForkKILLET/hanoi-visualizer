#include "core/graphic.hpp"
#include "systems/text_render.hpp"
#include "comps/text.hpp"
#include "comps/box.hpp"

void TextRenderSystem::update() {
    for (auto $text : entities()) {
        auto text = ecs.get_comp<TextComp>($text);
        auto bound = ecs.get_comp<BoundComp>($text);

        graphic::draw_text(
            text->text,
            bound->pos,
            text->color,
            text->font_size
        );
    }
}