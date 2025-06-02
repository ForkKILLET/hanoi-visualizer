#include <variant>
#include <vector>
#include "core/ecs.hpp"
#include "core/lens.hpp"
#include "utils/delegate.hpp"
#include "utils/vector.hpp" // IWYU pragma: keep
#include "utils/timing_func.hpp"

using namespace vec;

template <typename T>
concept interpolatable = requires(T a, T b, float k) {
    { (a + b) } -> std::convertible_to<T>;
    { (a - b) } -> std::convertible_to<T>;
    { (a * k) } -> std::convertible_to<T>;
};

template <typename T>
requires interpolatable<T>
struct AnimationTrack {
    CompLens<T> property;
    T from;
    T to;
    TimingFunc timing_func;
};

struct AnimationState {
    float time_elapsed = 0.0f;
    bool is_reversed = false;
    bool is_finished = false;
};

enum AnimationMode {
    ANIMATION_MODE_ONCE,
    ANIMATION_MODE_LOOP,
    ANIMATION_MODE_LOOP_REVERSE,
};

using AnyAnimationTrack = std::variant<
    AnimationTrack<float>,
    AnimationTrack<Vector2>
>;

struct Animation {
    float delay;
    float duration;
    AnimationMode mode;
    AnyAnimationTrack track;
    AnimationState state {};

    Delegate<> on_finish {};
};

struct AnimationComp : public Comp {
    AnimationComp(Animation animation) : 
        animations({ std::move(animation) }) {}
    AnimationComp(std::vector<Animation> animations) : 
        animations(std::move(animations)) {}

    std::vector<Animation> animations;
};
