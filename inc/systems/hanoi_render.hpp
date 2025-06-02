#include "comps/disabled.hpp"
#include "comps/hanoi.hpp"
#include "comps/box.hpp"
#include "core/ecs.hpp"

class HanoiRenderSystem : public System {
public:
    HanoiRenderSystem(ECS& ecs) : System(ecs) {
        with_comp<HanoiComp>();
        with_comp<BoundComp>();
        without_comp<DisabledComp>();
    }

    void update() override;
};