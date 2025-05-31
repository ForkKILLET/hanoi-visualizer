#pragma once
#include "core/ecs.hpp"
#include "comps/box.hpp"
#include "comps/button.hpp"
#include "comps/clickable.hpp"
#include "comps/text.hpp"

class ButtonSystem : public System {
public:
    ButtonSystem(ECS& ecs) : System(ecs) {
        with_comp<BorderComp>();
        with_comp<ButtonComp>();
    }

    void update_button_style(
        ButtonCompPtr& button, ClickableCompPtr& clickable, BorderCompPtr& border, TextCompPtr& text
    );

    void update() override;
};