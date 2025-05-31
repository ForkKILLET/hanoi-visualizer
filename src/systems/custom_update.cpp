#include "systems/custom_update.hpp"

void CustomUpdateSystem::update() {
    for (auto entity : entities()) {
        auto updater = ecs.get_comp<UpdaterComp>(entity);
        updater->updater(entity);
    }
}