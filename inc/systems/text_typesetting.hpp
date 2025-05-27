#pragma once
#include "ecs.hpp"

class TextTypesettingSystem : public System {
    using System::System;

    void update() override;
};