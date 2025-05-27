#include <raylib.h>
#include "systems/click.hpp"
#include "comps/clickable.hpp"
#include "comps/transform.hpp"

void ClickSystem::update() {
    Vector2 mouse_pos = GetMousePosition();
    bool is_left_clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (! is_left_clicked) return;

    for (auto entity : entities()) {
        auto clickable = ecs.get_comp<ClickableComp>(entity);
        auto bound = ecs.get_comp<BoundComp>(entity);
        if (CheckCollisionPointRec(mouse_pos, bound->to_rec())) {
            clickable->on_click();
        }
    }
}