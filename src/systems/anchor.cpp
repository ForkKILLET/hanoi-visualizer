#include "core/vector.hpp"
#include "systems/anchor.hpp"
#include "comps/box.hpp"

void AnchorSystem::update() {
    for (auto $anchor : entities()) {
        auto anchor = ecs.get_comp<AnchorComp>($anchor);
        auto bound = ecs.get_comp<BoundComp>($anchor);
        bound->pos = anchor->pos - bound->size.to_vec() * anchor->anchor;
    }
}