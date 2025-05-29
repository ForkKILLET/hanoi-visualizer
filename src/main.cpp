#include <raylib.h>
#include "comps/button.hpp"
#include "comps/hanoi.hpp"
#include "comps/hanoi_disk.hpp"
#include "core/ecs.hpp"
#include "entities/text.hpp"
#include "entities/hanoi.hpp"
#include "entities/button.hpp"
#include "entities/clickable.hpp"
#include "comps/text.hpp"
#include "comps/box.hpp"
#include "comps/clickable.hpp"
#include "systems/animation.hpp"
#include "systems/button.hpp"
#include "systems/click.hpp"
#include "systems/hanoi.hpp"
#include "systems/hanoi_disk_render.hpp"
#include "systems/text_render.hpp"
#include "systems/border_render.hpp"
#include "systems/anchor.hpp"
#include "systems/box_model.hpp"
#include "systems/text_typesetting.hpp"
#include "systems/timer.hpp"
#include "systems/hanoi_render.hpp"
#include "utils/hanoi.hpp"

class App {
public:
    ECS ecs {};

    void start() {
        ecs.register_comp<TextComp>();
        ecs.register_comp<BoundComp>();
        ecs.register_comp<AnchorComp>();
        ecs.register_comp<ClickableComp>();
        ecs.register_comp<HanoiComp>();
        ecs.register_comp<HanoiDiskComp>();
        ecs.register_comp<TimerComp>();
        ecs.register_comp<AnimationComp>();
        ecs.register_comp<BorderComp>();
        ecs.register_comp<PaddingComp>();
        ecs.register_comp<ButtonStyleComp>();

        auto anchor_system = ecs.register_system<AnchorSystem>();
        auto text_typesetting_system = ecs.register_system<TextTypesettingSystem>();
        auto text_render_system = ecs.register_system<TextRenderSystem>();
        auto border_render_system = ecs.register_system<BorderRenderSystem>();
        auto box_model_system = ecs.register_system<BoxModelSystem>();
        auto button_system = ecs.register_system<ButtonSystem>();
        auto hanoi_system = ecs.register_system<HanoiSystem>();
        auto hanoi_render_system = ecs.register_system<HanoiRenderSystem>();
        auto hanoi_disk_render_system = ecs.register_system<HanoiDiskRenderSystem>();
        auto timer_system = ecs.register_system<TimerSystem>();
        auto click_system = ecs.register_system<ClickSystem>();
        auto animation_system = ecs.register_system<AnimationSystem>();

        Entity $title_text = ecs.build_entity()
            .use<TextBuilder>()
                .text("Hanoi Visualizer")
                .text_color(BLACK)
                .font_size(25)
                .anchor({ 400, 20 }, TOP_CENTER)
            .build();

        Entity $hanoi = ecs.build_entity()
            .use<HanoiBuilder>()
                .disk_count(5)
                .size({ HANOI_WIDTH, 0 })
                .anchor({ 400, 60 }, TOP_CENTER)
            .build();

        Entity $start_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("Start")
                .anchor({ 40, 20 }, TOP_LEFT)
            .as<ClickableBuilder>()
                .on_click([&]() {
                    auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
                    hanoi->is_playing = ! hanoi->is_playing;
                    auto text = ecs.get_comp<TextComp>($start_button);
                    text->text = hanoi->is_playing ? "Pause" : "Start";
                })
            .build();

        Entity $step_prev_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("<-")
                .anchor({ 190 - 30, 20 }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&]() {
                    auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
                    auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);
                    if (hanoi->is_playing) hanoi->is_playing = false;
                    hanoi_system->step_prev(hanoi, hanoi_bound);
                })
            .build();

        Entity $step_next_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("->")
                .anchor({ 190 + 30, 20 }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&]() {
                    auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
                    auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);
                    if (hanoi->is_playing) hanoi->is_playing = false;
                    hanoi_system->step_next(hanoi, hanoi_bound);
                })
            .build();

        InitWindow(800, 450, "Hanoi Visualizer");

        while (! WindowShouldClose()) {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                animation_system->update();

                text_typesetting_system->update();
                anchor_system->update();
                box_model_system->update();

                hanoi_system->update();

                text_render_system->update();
                border_render_system->update();
                button_system->update();
                hanoi_render_system->update();
                hanoi_disk_render_system->update();

                timer_system->update();
                click_system->update();
            EndDrawing();
        }

        CloseWindow();
    }
};

int main() {
    App app {};

    app.start();

    return 0;
}