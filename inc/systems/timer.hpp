#include "core/ecs.hpp"
#include "comps/timer.hpp"

class TimerSystem : public System {
public:
    TimerSystem(ECS& ecs) : System(ecs) {
        with_comp<TimerComp>();
    }

    void update() override;
};