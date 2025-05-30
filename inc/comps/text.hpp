#pragma once
#include <raylib.h>
#include <string>
#include "core/ecs.hpp"

struct TextComp : public Comp {
    TextComp() = default;

    std::string text {};
    Color color { BLACK };
    int font_size { 20 };
};
using TextCompPtr = std::shared_ptr<TextComp>;