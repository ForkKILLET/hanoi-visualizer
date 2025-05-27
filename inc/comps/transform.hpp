#pragma once
#include <raylib.h>
#include "ecs.hpp"

struct Size {
    Size() = default;
    Size(float width, float height) :
        width(width), height(height) {}

    float width = 0.0f;
    float height = 0.0f;

    Vector2 to_vector() const {
        return {width, height};
    }
};

struct BoundComp : public Comp {
    BoundComp() = default;
    BoundComp(Vector2 pos) :
        pos(pos) {}

    Vector2 pos;
    Size size;

    Rectangle to_rec() const {
        return {pos.x, pos.y, size.width, size.height};
    }
};

struct AnchorComp : public Comp {
    AnchorComp() = default;
    AnchorComp(Vector2 pos, Vector2 anchor) :
        pos(pos), anchor(anchor) {};

    Vector2 pos;
    Vector2 anchor;
};

constexpr Vector2 CENTER_CENTER = {0.5f, 0.5f};
constexpr Vector2 TOP_CENTER = {0.5f, 0.0f};
constexpr Vector2 BOTTOM_CENTER = {0.5f, 1.0f};
constexpr Vector2 CENTER_LEFT = {0.0f, 0.5f};
constexpr Vector2 CENTER_RIGHT = {1.0f, 0.5f};
constexpr Vector2 TOP_LEFT = {0.0f, 0.0f};
constexpr Vector2 TOP_RIGHT = {1.0f, 0.0f};
constexpr Vector2 BOTTOM_LEFT = {0.0f, 1.0f};
constexpr Vector2 BOTTOM_RIGHT = {1.0f, 1.0f};
