#include <optional>
#include "systems/button.hpp"
#include "comps/button.hpp"
#include "comps/clickable.hpp"
#include "comps/text.hpp"
#include "utils/optional.hpp"

using namespace optional_ex;

void ButtonSystem::update_button_style(
    ButtonCompPtr& button,  ClickableCompPtr& clickable, BorderCompPtr& border, TextCompPtr& text
) {
    std::optional<Color> color {};
    if (button->is_disabled) try_assign(color, button->disabled_color);
    else if (clickable->is_active) try_assign(color, button->active_color);
    else if (clickable->is_hovering) try_assign(color, button->hover_color);
    else try_assign(color, button->normal_color);
    
    try_assign(border->color, color);
    if (text) try_assign(text->color, color);
}

void ButtonSystem::update() {
    for (auto $button : entities()) {
        auto button = ecs.get_comp<ButtonComp>($button);
        auto clickable = ecs.get_comp_or_emplace<ClickableComp>($button);
        auto border = ecs.get_comp<BorderComp>($button);
        auto text = ecs.get_comp_or_null<TextComp>($button);

        update_button_style(button, clickable, border, text);
    }
}