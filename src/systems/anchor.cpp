#include "systems/anchor.hpp"
#include "comps/transform.hpp"
#include "utils.hpp"

void AnchorSystem::update() {
    for (auto entity : entities()) {
        auto anchor = ecs.get_comp<AnchorComp>(entity);
        auto bound = ecs.get_comp<BoundComp>(entity);
        bound->pos = anchor->pos - anchor->anchor * bound->size.to_vector();
    }
}