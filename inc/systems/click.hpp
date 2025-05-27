#pragma once
#include "ecs.hpp"

class ClickSystem : public System {
public:
    using System::System;

    void update() override;
};