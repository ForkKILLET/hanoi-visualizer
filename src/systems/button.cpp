#include "systems/button.hpp"
#include "comps/button.hpp"
#include "comps/clickable.hpp"
#include "comps/text.hpp"
#include "utils/optional.hpp"

using namespace optional_ex;

void ButtonSystem::update() {
    for (auto $button : entities()) {
        auto button = ecs.get_comp<ButtonStyleComp>($button);
        if (button->is_inited) continue;
        button->is_inited = true;

        auto clickable = ecs.get_comp_or_emplace<ClickableComp>($button);
        auto border = ecs.get_comp<BorderComp>($button);
        auto text = ecs.get_comp_or_null<TextComp>($button);

        try_assign(border->color, button->normal_color);

        clickable->on_mouse_down += [=] {
            auto color = button->active_color;
            try_assign(border->color, color);
            if (text) try_assign(text->color, color);
        };
        clickable->on_mouse_up += [=] {
            auto color = clickable->is_hovering ? button->hover_color : button->normal_color;
            try_assign(border->color, color);
            if (text) try_assign(text->color, color);
        };
        clickable->on_mouse_enter += [=] {
            auto color = clickable->is_active ? button->active_color : button->hover_color;
            try_assign(border->color, color);
            if (text) try_assign(text->color, color);
        };
        clickable->on_mouse_leave += [=] {
            auto color = button->normal_color;
            try_assign(border->color, color);
            if (text) try_assign(text->color, color);
        };
    }
}