#include <string>
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
#include "utils/delegate.hpp"
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
                .font_size(25)
                .anchor({ 400, 20 }, TOP_CENTER)
            .build();

        Entity $hanoi = ecs.build_entity()
            .use<HanoiBuilder>()
                .disk_count(5)
                .size({ HANOI_WIDTH, 0 })
                .anchor({ 400, 100 }, TOP_CENTER)
            .build();

        auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
        auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);

        Delegate<> on_start_button_click {};
        Entity $start_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("Start")
                .anchor({ 40, 20 }, TOP_LEFT)
            .as<ClickableBuilder>()
                .on_click(on_start_button_click)
            .build();
        auto start_button_text = ecs.get_comp<TextComp>($start_button);

        auto start = [&, hanoi, start_button_text] {
            hanoi->is_playing = true;
            start_button_text->text = "Pause";
        };
        auto pause = [&, hanoi, start_button_text] {
            hanoi->is_playing = false;
            start_button_text->text = "Start";
        };
        on_start_button_click += [&] {
            if (hanoi->is_playing) pause();
            else start();
        };

        Entity $step_prev_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("<-")
                .anchor({ 190 - 33, 20 }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&, hanoi, hanoi_bound] {
                    if (hanoi->is_playing) pause();
                    hanoi_system->step_prev(hanoi, hanoi_bound);
                })
            .build();

        Entity $step_next_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("->")
                .anchor({ 190 + 33, 20 }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&, hanoi, hanoi_bound] {
                    if (hanoi->is_playing) pause();
                    hanoi_system->step_next(hanoi, hanoi_bound);
                })
            .build();

        DiskId disk_count;

        Entity $disk_count_text = ecs.build_entity()
            .use<TextBuilder>()
                .anchor({ 610, 20 }, TOP_CENTER)
            .build();

        auto disk_count_text = ecs.get_comp<TextComp>($disk_count_text);

        auto set_disk_count = [&disk_count, disk_count_text](DiskId count) {
            disk_count = count;
            disk_count_text->text = std::to_string(count);
        };
        set_disk_count(5);

        Entity $disk_dec_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("-")
                .anchor({ 610 - 40, 20 }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&] {
                    if (disk_count > 1) set_disk_count(disk_count - 1);
                })
            .build();

        Entity $disk_inc_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("+")
                .anchor({ 610 + 40, 20 }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&] {
                    if (disk_count < 16) set_disk_count(disk_count + 1);
                })
            .build();

        Entity $reset_button = ecs.build_entity()
            .use<ButtonBuilder>()
                .text("Reset")
                .anchor({ 760, 20 }, TOP_RIGHT)
            .as<ClickableBuilder>()
                .on_click([&, hanoi, hanoi_bound] {
                    hanoi_system->reset(hanoi, hanoi_bound, disk_count);
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