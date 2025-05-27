#pragma once
#include "ecs.hpp"

class AnchorSystem : public System {
    using System::System;

    void update() override;
};
