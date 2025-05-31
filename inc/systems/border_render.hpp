#include "core/graphic.hpp"
#include "core/ecs.hpp"
#include "comps/box.hpp"

class BorderRenderSystem : public System {
public:
    BorderRenderSystem(ECS& ecs) : System(ecs) {
        with_comp<BorderComp>();
        with_comp<BoundComp>();
    }

    void update() override {
        for (auto entity : entities()) {
            auto border = ecs.get_comp<BorderComp>(entity);
            auto bound = ecs.get_comp<BoundComp>(entity);

            graphic::stroke_rect(bound->get_outer_rect(), border->color);
        }
    }
};