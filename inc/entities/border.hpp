#include "core/ecs.hpp"
#include "comps/box.hpp"
#include "entities/builder.hpp"

class BorderBuilder : virtual public EntityBuilder {
public:
    using EntityBuilder::EntityBuilder;

    decltype(auto) border_width(VV width) {
        border_.width = width;
        return *this;
    }

    decltype(auto) border_color(Color color) {
        border_.color = color;
        return *this;
    }

    Entity build() override {
        auto entity = EntityBuilder::build();
        ecs.emplace_comp<BorderComp>(entity, border_);
        return entity;
    }

protected:
    BorderComp border_;
};