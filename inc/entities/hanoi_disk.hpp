#include "entities/pos.hpp"
#include "comps/hanoi_disk.hpp"

class HanoiDiskBuilder : virtual public BoundBuilder {
public:
    using BoundBuilder::BoundBuilder;   

    decltype(auto) id(DiskId id) {
        disk_.id = id;
        disk_.id_str = std::to_string(id + 1);
        return *this;
    }

    decltype(auto) is_phantom() {
        disk_.is_phantom = true;
        return *this;
    }

    Entity build() {
        BoundBuilder::build();
        ecs.emplace_comp<HanoiDiskComp>(entity, disk_);
        return entity;
    }

protected:
    HanoiDiskComp disk_ {};
};