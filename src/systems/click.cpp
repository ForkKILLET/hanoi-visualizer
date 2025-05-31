#include <raylib.h>
#include "core/vector.hpp"
#include "systems/click.hpp"
#include "comps/clickable.hpp"
#include "comps/box.hpp"

void ClickSystem::update() {
    VV2 old_mouse_pos = mouse_pos_;
    mouse_pos_ = VV2::from_real(GetMousePosition());

    bool is_left_pressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    for (auto $clickable : entities()) {
        auto clickable = ecs.get_comp<ClickableComp>($clickable);
        auto bound = ecs.get_comp<BoundComp>($clickable);

        bool was_hovering = clickable->is_hovering;
        bool is_hovering = clickable->is_hovering = CheckCollisionPointRec(mouse_pos_.to_real(), bound->get_outer_rec());
        if (! was_hovering && is_hovering) clickable->on_mouse_enter();
        else if (was_hovering && ! is_hovering) clickable->on_mouse_leave();

        bool was_active = clickable->is_active;
        bool is_active = clickable->is_active = (was_active || is_hovering) && is_left_pressed;
        if (! was_active && is_active) {
            clickable->on_mouse_down();
            clickable->mouse_down_pos = mouse_pos_;
        }
        else if (was_active && ! is_active) {
            clickable->on_mouse_up();
            float mouse_distance = magnitude(
                mouse_pos_ - clickable->mouse_down_pos.value_or(VV2 {}) 
            );
            if (mouse_distance < MAX_CLICK_DISTANCE) clickable->on_click();
        }
    }
}