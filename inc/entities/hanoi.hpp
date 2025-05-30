#include "core/ecs.hpp"
#include "entities/pos.hpp"
#include "comps/box.hpp"
#include "comps/hanoi.hpp"
#include "comps/timer.hpp"
#include "systems/hanoi.hpp"
#include "utils/delegate.hpp"

class HanoiBuilder : public BoundBuilder {
public:
    using BoundBuilder::BoundBuilder;

    decltype(auto) disk_count(DiskId disk_count) {
        hanoi_.disk_count = disk_count;
        return *this;
    }

    Entity build() override {
        BoundBuilder::build();
        auto hanoi_system = ecs.get_system<HanoiSystem>();
        auto hanoi = ecs.emplace_comp<HanoiComp>(entity, hanoi_);
        auto hanoi_bound = ecs.get_comp<BoundComp>(entity);
        ecs.emplace_comp<TimerComp>(entity, Delegate<> {
            [=] {
                if (hanoi->is_playing) hanoi_system->step_next(hanoi, hanoi_bound);
            }
        }, 0.6f);
        return entity;
    }

private:
    HanoiComp hanoi_ {};
};
