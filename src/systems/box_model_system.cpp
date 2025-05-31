#include "systems/box_model.hpp"
#include "comps/box.hpp"

void BoxModelSystem::update() {
    for (auto entity : entities()) {
        auto bound = ecs.get_comp<BoundComp>(entity);
        auto padding = ecs.get_comp_or_null<PaddingComp>(entity);
        auto border = ecs.get_comp_or_null<BorderComp>(entity);

        bound->outer_pos = bound->pos;
        bound->outer_size = bound->size;

        if (padding) {
            bound->outer_pos.x -= padding->left;
            bound->outer_pos.y -= padding->top;
            bound->outer_size.width += padding->left + padding->right;
            bound->outer_size.height += padding->top + padding->bottom;
        }
        if (border) {
            bound->outer_pos.x -= border->width;
            bound->outer_pos.y -= border->width;
            bound->outer_size.width += border->width * 2;
            bound->outer_size.height += border->width * 2;
        }
    }
}