#include "core/ecs.hpp"
#include "comps/box.hpp"

class BoxModelSystem : public System {
public:
    BoxModelSystem(ECS& ecs) : System(ecs) {
        with_comp<BoundComp>();
    }

    void update() override;
};