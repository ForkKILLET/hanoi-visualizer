#pragma once
#include <raylib.h>
#include "core/ecs.hpp"
#include "entities/text.hpp"
#include "entities/padding.hpp"
#include "entities/border.hpp"
#include "entities/clickable.hpp"
#include "entities/button_style.hpp"
#include "entities/pos.hpp"

class TextButtonBuilder :
    public TextBuilder,
    public PaddingBuilder, 
    public BorderBuilder, 
    public ClickableBuilder,
    public ButtonBuilder
{
public:
    TextButtonBuilder(ECS& ecs, Entity entity) :
        EntityBuilder(ecs, entity),
        BoundBuilder(ecs, entity),
        TextBuilder(ecs, entity),
        PaddingBuilder(ecs, entity),
        BorderBuilder(ecs, entity),
        ClickableBuilder(ecs, entity),
        ButtonBuilder(ecs, entity)
    {
        text_color(BLACK);
        font_size(20);
        padding_horizontal(10);
        padding_vertical(5);
        border_width(1);
        normal_color(BLACK);
        disabled_color(LIGHTGRAY);
        active_color(DARKBLUE);
        hover_color(BLUE);
    }

    Entity build() override {
        TextBuilder::build();
        PaddingBuilder::build();
        BorderBuilder::build();
        ClickableBuilder::build();
        ButtonBuilder::build();
        return entity;
    }
};
