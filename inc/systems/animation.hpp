#pragma once
#include "core/ecs.hpp"
#include "comps/animation.hpp"
#include "comps/disabled.hpp"

class AnimationSystem : public System {
public:
    AnimationSystem(ECS& ecs) : System(ecs) {
        with_comp<AnimationComp>();
        without_comp<DisabledComp>();
    }

    void update_anim(Entity entity, Animation& anim, float time_delta);

    template <typename T>
    requires interpolatable<T>
    void update_anim_track(Entity entity, Animation& anim, AnimationTrack<T>& track, float progress);

    void update() override;
};