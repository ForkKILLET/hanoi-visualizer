#include <optional>
#include <raylib.h>
#include "core/ecs.hpp"
#include "systems/click.hpp"
#include "comps/clickable.hpp"
#include "comps/box.hpp"
#include "utils/vector.hpp"
#include "services/graphic.hpp"

using namespace vec;

void ClickSystem::update() {
    auto graphic = ecs.get_service<GraphicService>();
    Vector2 mouse_pos = graphic->get_mouse_pos();

    bool is_left_pressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    std::optional<Entity> hovered_entity {};

    for (auto $clickable : entities()) {
        auto clickable = ecs.get_comp<ClickableComp>($clickable);
        auto bound = ecs.get_comp<BoundComp>($clickable);

        bool was_hovering = clickable->is_hovering;
        bool is_hovering = clickable->is_hovering = CheckCollisionPointRec(mouse_pos, bound->get_outer_rect());
        if (! was_hovering && is_hovering) {
            clickable->on_mouse_enter($clickable);
            hovered_entity = $clickable;
        }
        else if (was_hovering && ! is_hovering) clickable->on_mouse_leave($clickable);

        if (is_hovering) {
            hovered_entity = $clickable;
        }

        bool was_active = clickable->is_active;
        bool is_active = clickable->is_active = (was_active || is_hovering) && is_left_pressed;
        if (! was_active && is_active) {
            clickable->on_mouse_down($clickable);
            clickable->mouse_down_pos = mouse_pos;
        }
        else if (was_active && ! is_active) {
            clickable->on_mouse_up($clickable);
            float mouse_distance = vec_length(
                mouse_pos - clickable->mouse_down_pos.value_or(Vector2 {}) 
            );
            if (! clickable->is_disabled && mouse_distance < MAX_CLICK_DISTANCE)
                clickable->on_click($clickable);
        }
    }

    int cursor = MOUSE_CURSOR_DEFAULT;
    if (hovered_entity) {
        auto clickable = ecs.get_comp_or_null<ClickableComp>(*hovered_entity);
        if (clickable && ! clickable->is_disabled) cursor = MOUSE_CURSOR_POINTING_HAND;
    }
    graphic->set_cursor(cursor);
}