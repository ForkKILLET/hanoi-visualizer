#pragma once
#include "utils/delegate.hpp"
#include "core/ecs.hpp"

struct TimerComp : public Comp {
    TimerComp(Delegate<> on_timeout, float time_limit, bool is_infinite = true) :
        on_timeout(on_timeout),
        time_limit(time_limit),
        is_infinite(is_infinite)
    {}

    float time_elapsed {0.0f};
    bool is_running {true};

    float time_limit;
    bool is_infinite {true};
    Delegate<> on_timeout;
};