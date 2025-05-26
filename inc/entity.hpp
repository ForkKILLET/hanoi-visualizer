#pragma once
#include <functional>
#include <raylib.h>

template <typename E>
using OnClickHandler = std::function<void (E&)>;
using OnClickHandler_ = std::function<void ()>;

class Entity {
public:
    virtual void draw() const {}
    virtual void handle_click(const Vector2& mousePos) {}
    virtual void update() = 0;
};

