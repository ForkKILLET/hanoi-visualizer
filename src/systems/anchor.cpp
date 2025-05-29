#include "systems/anchor.hpp"
#include "comps/box.hpp"
#include "utils/vector.hpp"

using namespace vec;

void AnchorSystem::update() {
    for (auto $anchor : entities()) {
        auto anchor = ecs.get_comp<AnchorComp>($anchor);
        auto bound = ecs.get_comp<BoundComp>($anchor);
        bound->pos = anchor->pos - anchor->anchor * bound->size.to_vector();
    }
}