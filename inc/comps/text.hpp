#pragma once
#include <raylib.h>
#include <string>
#include "core/ecs.hpp"

struct TextComp : public Comp {
    std::string text;
    Color color;
    int font_size;
};
using TextCompPtr = std::shared_ptr<TextComp>;