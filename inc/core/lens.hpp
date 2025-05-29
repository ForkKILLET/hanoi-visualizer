#include <concepts>
#include <functional>
#include "core/ecs.hpp"

template <typename T>
class CompLens {
public:
    using ValueType = T;
    using Getter = std::function<const T& (Entity)>;
    using Setter = std::function<void (Entity, T)>;

    template <typename C>
    requires std::derived_from<C, Comp>
    CompLens(ECS& ecs, T C::* property) :
        ecs(ecs),
        getter([&ecs, property](Entity entity) -> const T& {
            return ecs.get_comp<C>(entity).get()->*property;
        }),
        setter([&ecs, property](Entity entity, T value) {
            ecs.get_comp<C>(entity).get()->*property = value;
        })
    {}

    const T& get(Entity entity) const {
        return getter(entity);
    }
    void set(Entity entity, T value) {
        setter(entity, value);
    }
    
protected:
    ECS& ecs;

    Getter getter;
    Setter setter;
};