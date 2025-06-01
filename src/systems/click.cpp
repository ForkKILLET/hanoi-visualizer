#include <optional>
#include <raylib.h>
#include "core/ecs.hpp"
#include "systems/click.hpp"
#include "comps/button.hpp"
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
            clickable->on_mouse_enter();
            hovered_entity = $clickable;
        }
        else if (was_hovering && ! is_hovering) clickable->on_mouse_leave();

        if (is_hovering) {
            hovered_entity = $clickable;
        }

        bool was_active = clickable->is_active;
        bool is_active = clickable->is_active = (was_active || is_hovering) && is_left_pressed;
        if (! was_active && is_active) {
            clickable->on_mouse_down();
            clickable->mouse_down_pos = mouse_pos;
        }
        else if (was_active && ! is_active) {
            clickable->on_mouse_up();
            float mouse_distance = vec_length(
                mouse_pos - clickable->mouse_down_pos.value_or(Vector2 {}) 
            );
            if (mouse_distance < MAX_CLICK_DISTANCE) clickable->on_click();
        }
    }

    // TODO: split this into a separate system
    int cursor = MOUSE_CURSOR_DEFAULT;
    if (hovered_entity) {
        cursor = ecs.get_comp<ButtonComp>(*hovered_entity)->is_disabled
            ? MOUSE_CURSOR_NOT_ALLOWED
            : MOUSE_CURSOR_POINTING_HAND;
    }
    graphic->set_cursor(cursor);
}