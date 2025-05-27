#pragma once
#include <array>
#include <bitset>
#include <concepts>
#include <cstddef>
#include <format>
#include <memory>
#include <queue>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <utility>

constexpr size_t MAX_ENTITY_COUNT = 1024;
constexpr size_t MAX_COMP_COUNT = 64;
constexpr size_t MAX_SYSTEM_COUNT = 64;

using Entity = size_t;
using CompSig = std::bitset<MAX_COMP_COUNT>;

class System;
class ECS;

struct Comp {
    virtual ~Comp() = default;
};

struct ICompSet {
    virtual ~ICompSet() = default;
};

template <typename C>
class CompSet : public ICompSet {
public:
    using CPtr = std::shared_ptr<C>;

    template <typename... Args>
    CPtr& emplace_comp(Entity entity, Args&&... args) {
        prepare_add_comp(entity);
        return comps[size] = std::make_shared<C>(std::forward<Args>(args)...);
    }

    CPtr& get_comp(Entity entity) {
        check_entity(entity);
        check_comp_exists(entity);
        return comps[entity_to_index[entity]];
    }

    template <typename... Args>
    CPtr& get_comp_or_emplace(Entity entity, Args&&... args) {
        check_entity(entity);
        size_t index = entity_to_index[entity];
        if (index) return comps[entity_to_index[entity]];
        return prepare_add_comp(entity, args...);
    }

    void remove_comp(Entity entity) {
        check_entity(entity);
        size_t index = entity_to_index[entity];
        entity_to_index[entity] = 0;
        size --;
        index_to_entity[index] = index_to_entity[size];
        comps[index] = comps[size];
    }

private:
    std::array<Entity, MAX_ENTITY_COUNT + 1> index_to_entity {};
    std::array<Entity, MAX_ENTITY_COUNT + 1> entity_to_index {};
    std::array<CPtr, MAX_ENTITY_COUNT + 1> comps;
    size_t size = 0;

    void check_entity(Entity entity) {
        if (entity < 1 || entity > MAX_ENTITY_COUNT)
            throw std::runtime_error(std::format("Invalid entity ID: {}.", entity));
    }

    void check_comp_exists(Entity entity) {
        if (! entity_to_index[entity])
            throw std::runtime_error(std::format("Entity {} does not have a {} component.", entity, typeid(C).name()));
    }

    void prepare_add_comp(Entity entity) {
        if (size >= MAX_ENTITY_COUNT)
            throw std::runtime_error("Maximum number of entities reached.");
        if (entity_to_index[entity])
            throw std::runtime_error(std::format("Entity {} already has a {} component.", entity, typeid(C).name()));
        
        ++ size;
        entity_to_index[entity] = size;
        index_to_entity[size] = entity;
    }
};

class CompManager {
public:
    using CPtr = std::shared_ptr<Comp>;
    using CompSetPtr = std::shared_ptr<ICompSet>;

    template <typename C>
    requires std::derived_from<C, Comp>
    void register_comp() {
        std::type_index type = typeid(C);
        if (comp_indices.contains(type))
            throw std::runtime_error(std::format("Component type {} already registered.", type.name()));

        comp_indices[type] = comp_index ++;
        comp_sets[type] = std::make_shared<CompSet<C>>();
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    std::shared_ptr<CompSet<C>> get_comp_set() {
        std::type_index type = typeid(C);
        if (! comp_sets.contains(type))
            throw std::runtime_error(std::format("Component type {} not registered.", type.name()));

        return std::dynamic_pointer_cast<CompSet<C>>(comp_sets[type]);
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    size_t get_comp_index() const {
        std::type_index type = typeid(C);
        if (! comp_indices.contains(type))
            throw std::runtime_error(std::format("Component type {} not registered.", type.name()));

        return comp_indices.at(type);
    }

private:
    friend class ECS;

    std::unordered_map<std::type_index, size_t> comp_indices {};
    std::unordered_map<std::type_index, CompSetPtr> comp_sets {};
    size_t comp_index = 0;
};

class EntityManager {
public:
    using EntityExistsMap = std::array<bool, MAX_ENTITY_COUNT>;

    EntityManager() {
        for (size_t id = 1; id <= MAX_ENTITY_COUNT; ++ id)
            available_ids.push(id);
    }

    void assert_entity_exists(Entity entity) const {
        if (! entity_is_valid(entity))
            throw std::runtime_error(std::format("Entity {} does not exist.", entity));
    }

    bool entity_is_valid(Entity entity) const {
        if (entity < 1 || entity > MAX_ENTITY_COUNT)
            throw std::runtime_error(std::format("Invalid entity ID: {}.", entity));

        return entity_exists[entity];
    }

    bool entity_matches_sig(Entity entity, CompSig sig) const {
        assert_entity_exists(entity);
        return (entity_sigs[entity] & sig) == sig;
    }

    Entity create_entity() {
        if (available_ids.empty())
            throw std::runtime_error("Maximum number of entities reached.");

        Entity entity = available_ids.front();
        available_ids.pop();
        ++ living_entites_count;
        entity_exists[entity] = true;
        entity_sigs[entity] = 0;
        return entity;
    }

    void destroy_entity(Entity entity) {
        assert_entity_exists(entity);

        -- living_entites_count;
        entity_exists[entity] = false;
        available_ids.push(entity);
    }

private:
    friend class ECS;

    std::queue<Entity> available_ids {};
    size_t living_entites_count = 0;
    EntityExistsMap entity_exists {};
    std::array<CompSig, MAX_ENTITY_COUNT> entity_sigs {};
};

class EntityView {
public:
    class Iterator {
    public:
        Iterator(Entity entity, const CompSig& sig, ECS& ecs) :
            entity(entity),
            sig(sig),
            ecs(ecs)
        {}

        Entity operator*() const { return entity; }
        Iterator& operator++();
        bool operator==(const Iterator& other) const {
            return entity == other.entity;
        }

    private:
        Entity entity;
        const CompSig& sig;
        ECS& ecs;
    };

    EntityView(System& system, ECS& ecs) :
        system(system),
        ecs(ecs)
    {}

    Iterator begin();
    Iterator end();

private:
    System& system;
    ECS& ecs;
};

class System {
public:
    System(ECS& ecs) : ecs(ecs), sig_() {}

    template <typename... Cs>
    requires (std::derived_from<Cs, Comp> && ...)
    decltype(auto) with_comp();

    const CompSig& sig() { return sig_; }

    EntityView entities();
    virtual void update() = 0;

protected:
    ECS& ecs;

private:
    CompSig sig_;
};

class SystemManager {
public:
    SystemManager(ECS& ecs) : ecs(ecs) {}

    template <typename S>
    requires std::derived_from<S, System>
    std::shared_ptr<S> register_system() {
        std::type_index system_type = typeid(S);
        if (systems.contains(system_type))
            throw std::runtime_error(std::format("System {} already registered.", system_type.name()));

        std::shared_ptr<S> system = std::make_shared<S>(ecs);
        systems[system_type] = system;
        return system;
    }

private:
    ECS& ecs;
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems {};
    size_t system_index = 0;
};

class ECS {
public:
    template <typename C, typename... Args>
    requires std::derived_from<C, Comp>
    void emplace_comp(Entity entity, Args&&... args) {
        get_comp_set<C>()->emplace_comp(entity, std::forward<Args>(args)...);
        update_entity_sig<C, true>(entity);
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    void emplace_comp(Entity entity, C&& comp) {
        get_comp_set<C>()->emplace_comp(entity, std::forward<C>(comp));
        update_entity_sig<C, true>(entity);
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    void remove_comp(Entity entity) {
        get_comp_set<C>()->remove_comp(entity);
        update_entity_sig<C, false>(entity);
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    std::shared_ptr<C>& get_comp(Entity entity) {
        return get_comp_set<C>()->get_comp(entity);
    }

    template <typename C, typename... Args>
    requires std::derived_from<C, Comp>
    std::shared_ptr<C>& get_comp_or_emplace(Entity entity, Args&&... args) {
        return get_comp_set<C>()->get_comp_or_emplace(entity, std::forward<Args>(args)...);
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    size_t get_comp_index() const {
        return comp_manager_.get_comp_index<C>();
    }

    Entity create_entity() {
        return entity_manager_.create_entity();
    }
    void destroy_entity(Entity entity) {
        entity_manager_.destroy_entity(entity);
    }

    const EntityManager::EntityExistsMap& entity_exists_map() const {
        return entity_manager_.entity_exists;
    }
    bool entity_matches_sig(Entity entity, CompSig sig) const {
        return entity_manager_.entity_matches_sig(entity, sig);
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    void register_comp() {
        comp_manager_.register_comp<C>();
    }

    template <typename S>
    requires std::derived_from<S, System>
    std::shared_ptr<S> register_system() {
        return system_manager_.register_system<S>();
    }

private:
    CompManager comp_manager_ {};
    EntityManager entity_manager_ {};
    SystemManager system_manager_ { *this };

    template <typename C, bool value>
    requires std::derived_from<C, Comp>
    void update_entity_sig(Entity entity) {
        size_t comp_index = comp_manager_.get_comp_index<C>();
        entity_manager_.entity_sigs[entity].set(comp_index, value);
    }

    template <typename C>
    requires std::derived_from<C, Comp>
    std::shared_ptr<CompSet<C>> get_comp_set() {
        return comp_manager_.get_comp_set<C>();
    }
};

template <typename... Cs>
requires (std::derived_from<Cs, Comp> && ...)
decltype(auto) System::with_comp() {
    ((sig_.set(ecs.get_comp_index<Cs>(), true)), ...);
    return this;
}

inline EntityView System::entities() {
    return EntityView(*this, ecs);
}

inline EntityView::Iterator EntityView::begin() {
    auto it = Iterator(0, system.sig(), ecs);
    ++ it;
    return it;
}
inline EntityView::Iterator EntityView::end() {
    return Iterator(MAX_ENTITY_COUNT, system.sig(), ecs);
}

inline EntityView::Iterator& EntityView::Iterator::operator++() {
    auto& entity_exists = ecs.entity_exists_map();
    while (true) {
        ++ entity;
        if (entity == MAX_ENTITY_COUNT) break;
        if (! entity_exists[entity]) continue;
        if (ecs.entity_matches_sig(entity, sig)) break;
    }
    return *this;
}