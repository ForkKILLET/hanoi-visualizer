#include "entities/pos.hpp"
#include "comps/hanoi_disk.hpp"

class HanoiDiskBuilder : public BoundBuilder {
public:
    using BoundBuilder::BoundBuilder;   

    decltype(auto) id(DiskId id) {
        ecs.emplace_comp<HanoiDiskComp>(entity, id);
        return *this;
    }
};