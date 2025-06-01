#include "core/ecs.hpp"
#include "comps/text.hpp"
#include "comps/box.hpp"
#include "services/graphic.hpp"
#include "systems/text_typesetting.hpp"

void TextTypesettingSystem::update() {
    auto graphic = ecs.get_service<GraphicService>();

    for (auto $text : entities()) {
        auto text = ecs.get_comp<TextComp>($text);
        auto bound = ecs.get_comp<BoundComp>($text);

        bound->size = {
            static_cast<float>(graphic->measure_text_0(text->text, text->font_size)),
            static_cast<float>(text->font_size)
        };
    }
}