#pragma once
#include "core/ecs.hpp"
#include "entities/text.hpp"
#include "entities/padding.hpp"
#include "entities/border.hpp"
#include "entities/clickable.hpp"
#include "button_style.hpp"
#include "pos.hpp"
#include "raylib.h"

class ButtonBuilder :
    public TextBuilder,
    public PaddingBuilder, 
    public BorderBuilder, 
    public ClickableBuilder,
    public ButtonStyleBuilder
{
public:
    ButtonBuilder(ECS& ecs, Entity entity) :
        EntityBuilder(ecs, entity),
        BoundBuilder(ecs, entity),
        TextBuilder(ecs, entity),
        PaddingBuilder(ecs, entity),
        BorderBuilder(ecs, entity),
        ClickableBuilder(ecs, entity),
        ButtonStyleBuilder(ecs, entity)
    {
        text_color(BLACK);
        font_size(20);
        padding_horizontal(10);
        padding_vertical(5);
        border_width(1);
        normal_color(BLACK);
        active_color(RED);
        hover_color(LIGHTGRAY);
    }

    Entity build() override {
        TextBuilder::build();
        PaddingBuilder::build();
        BorderBuilder::build();
        ClickableBuilder::build();
        ButtonStyleBuilder::build();
        return entity;
    }
};
