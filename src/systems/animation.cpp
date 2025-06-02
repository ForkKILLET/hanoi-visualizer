#include <cmath>
#include <raylib.h>
#include <variant>
#include "core/ecs.hpp"
#include "systems/animation.hpp"

void AnimationSystem::update_anim(Entity entity, Animation& anim, float time_delta) {
    if (anim.state.is_finished) return;

    anim.state.time_elapsed += time_delta;
    float real_time_elapsed = anim.state.time_elapsed - anim.delay;

    if (real_time_elapsed < 0) return;
    float progress = real_time_elapsed / anim.duration;

    std::visit([&](auto& track) {
        update_anim_track(entity, anim, track, progress);
    }, anim.track);
}

template <typename T>
requires interpolatable<T>
void AnimationSystem::update_anim_track(
    Entity entity,
    Animation& anim,
    AnimationTrack<T>& track,
    float progress
) {
    T value_from, value_to, value;

    if (anim.state.is_reversed) {
        value_from = track.to;
        value_to = track.from;
    }
    else {
        value_from = track.from;
        value_to = track.to;
    }

    if (progress < 1.0f) {
        T value_range = value_to - value_from;
        value = value_from + value_range * track.timing_func(progress);
    }
    else {
        value = value_to;
        switch (anim.mode) {
            case ANIMATION_MODE_ONCE:
                anim.state.is_finished = true;
                anim.on_finish();
                break;
            case ANIMATION_MODE_LOOP_REVERSE:
                anim.state.is_reversed = ! anim.state.is_reversed;
            case ANIMATION_MODE_LOOP:
                anim.state.time_elapsed = anim.delay;
                break;
        }
    }

    track.property.set(entity, value);
}

void AnimationSystem::update() {
    float time_delta = GetFrameTime();

    for (auto entity : entities()) {
        auto& anims = ecs.get_comp<AnimationComp>(entity)->animations;
        for (auto& anim : anims) {
            update_anim(entity, anim, time_delta);
        }
    }
}