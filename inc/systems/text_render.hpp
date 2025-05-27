#pragma once
#include "ecs.hpp"

class TextRenderSystem : public System {
    using System::System;

    void update() override;
};