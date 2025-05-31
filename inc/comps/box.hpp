#pragma once
#include <raylib.h>
#include "core/ecs.hpp"
#include "core/vector.hpp"

struct Size {
    Size() = default;
    Size(VV width, VV height) :
        width(width), height(height) {}

    VV width { 0.0f };
    VV height { 0.0f };

    VV2 to_vec() const { return { width, height }; }
    operator VV2() const { return to_vec(); }
};

struct BoundComp : public Comp {
    BoundComp() = default;
    BoundComp(VV2 pos) :
        pos(pos), size() {}
    BoundComp(Size size) :
        pos(), size(size) {}
    BoundComp(VV2 pos, Size size) :
        pos(pos), size(size) {}
    BoundComp(Rectangle rec) :
        pos({ rec.x, rec.y }), size({ rec.width, rec.height }) {}

    VV2 pos;
    Size size;
    VV2 outer_pos;
    Size outer_size;

    Rectangle get_rec() const {
        return { pos.x, pos.y, size.width, size.height };
    }
    Rectangle get_outer_rec() const {
        return { outer_pos.x, outer_pos.y, outer_size.width, outer_size.height };
    }
};

struct AnchorComp : public Comp {
    AnchorComp() = default;
    AnchorComp(VV2 pos, Vector2 anchor) :
        pos(pos), anchor(anchor) {};

    VV2 pos;
    Vector2 anchor;
};

constexpr Vector2 CENTER_CENTER = { 0.5f, 0.5f };
constexpr Vector2 TOP_CENTER = { 0.5f, 0.0f };
constexpr Vector2 BOTTOM_CENTER = { 0.5f, 1.0f };
constexpr Vector2 CENTER_LEFT = { 0.0f, 0.5f };
constexpr Vector2 CENTER_RIGHT = { 1.0f, 0.5f };
constexpr Vector2 TOP_LEFT = { 0.0f, 0.0f };
constexpr Vector2 TOP_RIGHT = { 1.0f, 0.0f };
constexpr Vector2 BOTTOM_LEFT = { 0.0f, 1.0f };
constexpr Vector2 BOTTOM_RIGHT = { 1.0f, 1.0f };

struct BorderComp : public Comp {
    BorderComp() = default;
    VV width;
    Color color;
};

struct PaddingComp : public Comp {
    PaddingComp() = default;
    PaddingComp(VV padding) :
        top(padding), right(padding), bottom(padding), left(padding) {}
    PaddingComp(VV topBottom, VV leftRight) :
        top(topBottom), right(leftRight), bottom(topBottom), left(leftRight) {} 
    PaddingComp(VV top, VV right, VV bottom, VV left) :
        top(top), right(right), bottom(bottom), left(left) {}

    VV top;
    VV right;
    VV bottom;
    VV left;
};

using BoundCompPtr = std::shared_ptr<BoundComp>;
using AnchorCompPtr = std::shared_ptr<AnchorComp>;
using BorderCompPtr = std::shared_ptr<BorderComp>;
using PaddingCompPtr = std::shared_ptr<PaddingComp>;