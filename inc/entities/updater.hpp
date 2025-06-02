#include <functional>
#include "core/ecs.hpp"
#include "comps/updater.hpp"
#include "entities/builder.hpp"
#include "utils/delegate.hpp"

class UpdaterBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) on_update(std::function<void (Entity)> func) {
        on_update_ += func;
        return *this;
    }
    decltype(auto) on_update(Delegate<Entity>& func) {
        on_update_ = func;
        return *this;
    }

    Entity build() override {
        ecs.emplace_comp<UpdaterComp>(entity, on_update_);
        return EntityBuilder::build();
    }

protected:
    Delegate<Entity> on_update_ {};
};