#pragma once
#include <raylib.h>
#include <string>
#include "ecs.hpp"

struct TextComp : public Comp {
    std::string text;
    Color       color;
    int         font_size;
};