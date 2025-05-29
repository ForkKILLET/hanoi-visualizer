#pragma once
#include <functional>
#include <cmath>

using TimingFunc = std::function<float (float)>;

namespace TimingFuncs {
    inline float linear(float t) {
        return t;
    }

    inline float ease_in(float t) {
        return t * t;
    }

    inline float ease_out(float t) {
        return t * (2 - t);
    }

    inline float ease_in_out(float t) {
        return (t < 0.5f) ? (2 * t * t) : (1 - pow(-2 * t + 2, 2) / 2);
    }
};