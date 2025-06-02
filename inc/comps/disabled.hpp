#pragma once
#include "core/ecs.hpp"
#include "utils/functional.hpp"

using namespace fp;

struct DisabledComp : public Comp {};

// TODO: refa to ComputedComp

struct DynamicDisabledComp : public Comp {
    DynamicDisabledComp() = default;
    DynamicDisabledComp(Condition is_disabled) : is_disabled(is_disabled) {}

    Condition is_disabled { constant<false> };
};