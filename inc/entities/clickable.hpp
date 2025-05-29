#pragma once
#include "comps/clickable.hpp"
#include "entities/builder.hpp"
#include "utils/delegate.hpp"

class ClickableBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) on_click(UniqueFunction<void> func) {
        on_click_ += std::move(func);
        return *this;
    }

    Entity build() override {
        ecs.emplace_comp<ClickableComp>(entity, on_click_);
        return EntityBuilder::build();
    }

protected:
    Delegate<> on_click_ {};
};