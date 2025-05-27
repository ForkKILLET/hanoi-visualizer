#pragma once
#include "ecs.hpp"
#include "raylib.h"

struct PosComp : public Comp, Vector2 {
    PosComp() = default;
    PosComp(Vector2 pos) : Vector2(pos) {}
};

struct BoundComp : public PosComp {
    float width;
    float height;
};

struct AnchorComp : public Comp {
    AnchorComp() = default;
    AnchorComp(Vector2 pos, Vector2 anchor) :
        pos(pos), anchor(anchor) {};

    Vector2 pos;
    Vector2 anchor;
};