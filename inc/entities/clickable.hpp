#pragma once
#include <functional>
#include "core/ecs.hpp"
#include "comps/clickable.hpp"
#include "entities/builder.hpp"
#include "utils/delegate.hpp"

class ClickableBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) on_click(std::function<void ()> func) {
        on_click_ += [func](Entity) {
            func();
        };
        return *this;
    }
    decltype(auto) on_click(std::function<void (Entity)> func) {
        on_click_ += func;
        return *this;
    }
    decltype(auto) on_click(Delegate<Entity>& func) {
        on_click_ = func;
        return *this;
    }

    Entity build() override {
        ecs.emplace_comp<ClickableComp>(entity, on_click_);
        return EntityBuilder::build();
    }

protected:
    Delegate<Entity> on_click_ {};
};