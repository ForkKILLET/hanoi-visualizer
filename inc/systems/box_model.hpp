#include "core/ecs.hpp"
#include "comps/box.hpp"
#include "comps/disabled.hpp"

class BoxModelSystem : public System {
public:
    BoxModelSystem(ECS& ecs) : System(ecs) {
        with_comp<BoundComp>();
        without_comp<DisabledComp>();
    }

    void update() override;
};