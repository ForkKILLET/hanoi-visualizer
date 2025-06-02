#include <string>
#include <vector>
#include <raylib.h>
#include "core/ecs.hpp"
#include "entities/clock.hpp"
#include "entities/disable.hpp"
#include "entities/text.hpp"
#include "entities/hanoi.hpp"
#include "entities/text_button.hpp"
#include "entities/clickable.hpp"
#include "entities/updater.hpp"
#include "comps/clock.hpp"
#include "comps/disabled.hpp"
#include "comps/text.hpp"
#include "comps/box.hpp"
#include "comps/clickable.hpp"
#include "comps/updater.hpp"
#include "comps/button.hpp"
#include "comps/hanoi.hpp"
#include "comps/hanoi_disk.hpp"
#include "services/graphic.hpp"
#include "systems/animation.hpp"
#include "systems/button.hpp"
#include "systems/click.hpp"
#include "systems/clock.hpp"
#include "systems/clock_render.hpp"
#include "systems/custom_update.hpp"
#include "systems/disable.hpp"
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
#include "utils/functional.hpp"
#include "utils/hanoi.hpp"

const char VERSION[] =
    #include "version.txt"
;

class App {
public:
    ECS ecs {};

    void start() {
#ifndef DEBUG
        SetTraceLogLevel(LOG_ERROR);
#endif

        ecs.register_comp<DynamicDisabledComp>();
        ecs.register_comp<DisabledComp>();
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
        ecs.register_comp<ButtonComp>();
        ecs.register_comp<UpdaterComp>();
        ecs.register_comp<ClockComp>();

        auto graphic = ecs.register_service<GraphicService>();

        auto custom_update_system = ecs.register_system<CustomUpdateSystem>();
        auto disable_system = ecs.register_system<DisableSystem>();
        auto clock_system = ecs.register_system<ClockSystem>();
        auto anchor_system = ecs.register_system<AnchorSystem>();
        auto text_typesetting_system = ecs.register_system<TextTypesettingSystem>();
        auto text_render_system = ecs.register_system<TextRenderSystem>();
        auto border_render_system = ecs.register_system<BorderRenderSystem>();
        auto box_model_system = ecs.register_system<BoxModelSystem>();
        auto button_system = ecs.register_system<ButtonSystem>();
        auto hanoi_system = ecs.register_system<HanoiSystem>();
        auto hanoi_render_system = ecs.register_system<HanoiRenderSystem>();
        auto hanoi_disk_render_system = ecs.register_system<HanoiDiskRenderSystem>();
        auto clock_render_system = ecs.register_system<ClockRenderSystem>();
        auto timer_system = ecs.register_system<TimerSystem>();
        auto click_system = ecs.register_system<ClickSystem>();
        auto animation_system = ecs.register_system<AnimationSystem>();

        constexpr float Y_TOPBAR = 25;
        constexpr float Y_TOOLBAR = 80;

        Entity $title_text = ecs.build_entity()
            .use<TextBuilder>()
                .text("Hanoi Visualizer")
                .font_size(30)
                .anchor({ 400, 20 }, TOP_CENTER)
            .build();

        Delegate<> on_user_solve {};
        Entity $hanoi = ecs.build_entity()
            .use<HanoiBuilder>()
                .disk_count(5)
                .on_user_solve(on_user_solve)
                .size({ HANOI_WIDTH, 0 })
                .anchor({ 400, 200 }, TOP_CENTER)
            .build();

        auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
        auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);

        DiskId disk_count;

        Condition is_user_mode = [hanoi] { return hanoi->is_user_mode; };
        Condition is_demo_mode = [hanoi] { return ! hanoi->is_user_mode; };
        Condition is_playing = [hanoi] { return hanoi->is_playing; };
        Condition is_solved = [hanoi] { return hanoi->is_solved; };

        Delegate<Entity> on_start_button_click {};
        Entity $start_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("Start")
                .anchor({ 40, Y_TOOLBAR }, TOP_LEFT)
            .as<ClickableBuilder>()
                .on_click(on_start_button_click)
            .use<DisableBuilder>()
                .enable_when(f_or(is_demo_mode, f_not(is_playing)))
            .build();
        auto start_button_text = ecs.get_comp<TextComp>($start_button);

        Delegate<Entity> update_user_clock {};
        Entity $user_clock = ecs.build_entity()
            .use<ClockBuilder>()
            .use<TextBuilder>()
                .anchor({ 40, Y_TOOLBAR }, TOP_LEFT)
            .use<DisableBuilder>()
                .enable_when(f_and(is_user_mode, is_playing))
            .use<UpdaterBuilder>()
                .on_update(update_user_clock)
            .build();
        auto user_clock = ecs.get_comp<ClockComp>($user_clock);
        auto user_clock_text = ecs.get_comp<TextComp>($user_clock);

        auto start = [&, hanoi, start_button_text] {
            hanoi->is_playing = true;
            start_button_text->text = "Pause";
        };
        auto pause = [&, hanoi, start_button_text] {
            hanoi->is_playing = false;
            start_button_text->text = "Start";
        };

        on_start_button_click += [&, hanoi](Entity $self) {
            if (hanoi->is_user_mode) {
                if (! hanoi->is_playing) {
                    hanoi->is_playing = true;
                    user_clock->time_elapsed = 0;
                    user_clock->is_paused = false;
                }
            }
            else {
                if (hanoi->is_playing) pause();
                else start();
            }
        };

        auto reset = [&, hanoi, hanoi_bound, hanoi_system, user_clock] {
            pause();
            hanoi_system->reset(hanoi, hanoi_bound, disk_count, user_clock);
        };

        auto is_user_solved = f_and(is_user_mode, is_solved);

        Entity $clearance_cond_1 = ecs.build_entity()
            .use<TextBuilder>()
                .text("* Solve the Hanoi!")
                .text_color(ORANGE)
                .anchor({ 300, 120 }, TOP_LEFT)
            .use<DisableBuilder>()
                .enable_when(is_user_solved)
            .build();

        Entity $clearance_cond_2 = ecs.build_entity()
            .use<TextBuilder>()
                .text("* Speedrun!")
                .anchor({ 100, 120 }, TOP_LEFT)
            .use<DisableBuilder>()
                .enable_when(is_user_solved)
            .build();

        Entity $clearance_cond_3 = ecs.build_entity()
            .use<TextBuilder>()
                .text("* Minimum steps!")
                .anchor({ 550, 120 }, TOP_LEFT)
            .use<DisableBuilder>()
                .enable_when(is_user_solved)
            .build();

        constexpr float USER_TIME_PER_STEP = 1.0f;

        Condition is_min_step = [=] {
            const auto min_step_count = hanoi->solution.steps.size();
            return hanoi->user_step_count <= min_step_count;
        };
        Condition is_speedrun = [=] {
            const auto min_step_count = hanoi->solution.steps.size();
            return user_clock->time_elapsed <= min_step_count * USER_TIME_PER_STEP;
        };

        on_user_solve += [&] {
            ecs.get_comp<TextComp>($clearance_cond_2)->color = is_speedrun() ? ORANGE : GRAY;
            ecs.get_comp<TextComp>($clearance_cond_3)->color = is_min_step() ? ORANGE : GRAY;
        };

        update_user_clock += [&, user_clock, user_clock_text](Entity) {
            user_clock_text->color = is_speedrun() ? ORANGE : GRAY;
        };

        Entity $mode_demo_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("Demo")
                .anchor({ 40, Y_TOPBAR }, TOP_LEFT)
            .as<ClickableBuilder>()
                .on_click([&, hanoi] {
                    if (! hanoi->is_user_mode) return;
                    hanoi->is_user_mode = false;
                    reset();
                })
            .use<UpdaterBuilder>()
                .on_update([&](Entity $self) {
                    auto button = ecs.get_comp<ButtonComp>($self);
                    auto clickable = ecs.get_comp<ClickableComp>($self);
                    clickable->is_disabled = button->is_highlighted = ! hanoi->is_user_mode;
                })
            .build();

        Entity $mode_user_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("Play")
                .anchor({ 150, Y_TOPBAR }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&, hanoi] {
                    if (hanoi->is_user_mode) return;
                    hanoi->is_user_mode = true;
                    reset();
                })
            .use<UpdaterBuilder>()
                .on_update([&](Entity $self) {
                    auto button = ecs.get_comp<ButtonComp>($self);
                    auto clickable = ecs.get_comp<ClickableComp>($self);
                    clickable->is_disabled = button->is_highlighted = hanoi->is_user_mode;
                })
            .build();

        Entity $step_text_slash = ecs.build_entity()
            .use<TextBuilder>()
                .text("/")
                .anchor({ 400, Y_TOOLBAR }, TOP_CENTER)
            .build();
        Entity $step_text_current = ecs.build_entity()
            .use<TextBuilder>()
                .anchor({ 400 - 20, Y_TOOLBAR }, TOP_RIGHT)
            .use<UpdaterBuilder>()
                .on_update([&, hanoi](Entity $self) {
                    auto text = ecs.get_comp<TextComp>($self);
                    text->text = std::to_string(
                        hanoi->is_user_mode ? hanoi->user_step_count : hanoi->solution.cursor()
                    );
                })
            .build();
        Entity $step_text_total = ecs.build_entity()
            .use<TextBuilder>()
                .anchor({ 400 + 20, Y_TOOLBAR }, TOP_LEFT)
            .use<UpdaterBuilder>()
                .on_update([&, hanoi](Entity $self) {
                    auto text = ecs.get_comp<TextComp>($self);
                    text->text = std::to_string(hanoi->solution.steps.size());
                })
            .build();

        Entity $step_prev_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("<-")
                .anchor({ 300, Y_TOOLBAR }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&, hanoi, hanoi_bound] {
                    if (hanoi->is_playing) pause();
                    hanoi_system->step_prev(hanoi, hanoi_bound);
                })
            .use<UpdaterBuilder>()
                .on_update([&, hanoi](Entity $self) {
                    auto clickable = ecs.get_comp<ClickableComp>($self);
                    clickable->is_disabled = ! hanoi->solution.has_prev();
                })
            .use<DisableBuilder>()
                .enable_when(is_demo_mode)
            .build();

        Entity $step_next_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("->")
                .anchor({ 500, Y_TOOLBAR }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&, hanoi, hanoi_bound] {
                    if (hanoi->is_playing) pause();
                    hanoi_system->step_next(hanoi, hanoi_bound);
                })
            .use<UpdaterBuilder>()
                .on_update([&, hanoi](Entity $self) {
                    auto clickable = ecs.get_comp<ClickableComp>($self);
                    clickable->is_disabled = ! hanoi->solution.has_next();
                })
            .use<DisableBuilder>()
                .enable_when(is_demo_mode)
            .build();

        Entity $disk_count_text = ecs.build_entity()
            .use<TextBuilder>()
                .anchor({ 610, Y_TOOLBAR }, TOP_CENTER)
            .build();

        auto disk_count_text = ecs.get_comp<TextComp>($disk_count_text);

        auto set_disk_count = [&disk_count, disk_count_text](DiskId count) {
            disk_count = count;
            disk_count_text->text = std::to_string(count);
        };
        set_disk_count(5);

        Entity $disk_dec_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("-")
                .anchor({ 610 - 40, Y_TOOLBAR }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&] {
                    if (disk_count == MIN_DISK_COUNT) return;
                    set_disk_count(disk_count - 1);
                })
            .use<UpdaterBuilder>()
                .on_update([&](Entity $self) {
                    auto clickable = ecs.get_comp<ClickableComp>($self);
                    clickable->is_disabled = disk_count == MIN_DISK_COUNT;
                })
            .build();

        Entity $disk_inc_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("+")
                .anchor({ 610 + 40, Y_TOOLBAR }, TOP_CENTER)
            .as<ClickableBuilder>()
                .on_click([&] {
                    if (disk_count == MAX_DISK_COUNT) return;
                    set_disk_count(disk_count + 1);
                })
            .use<UpdaterBuilder>()
                .on_update([&](Entity $self) {
                    auto clickable = ecs.get_comp<ClickableComp>($self);
                    clickable->is_disabled = disk_count == MAX_DISK_COUNT;
                })
            .build();

        Entity $reset_button = ecs.build_entity()
            .use<TextButtonBuilder>()
                .text("Reset")
                .anchor({ 760, Y_TOOLBAR }, TOP_RIGHT)
            .as<ClickableBuilder>()
                .on_click(reset)
            .build();

        Entity $version_text = ecs.build_entity()
            .use<TextBuilder>()
                .text(VERSION)
                .text_color(BLUE)
                .anchor({ 780, 430 }, BOTTOM_RIGHT)
            .build();

        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        graphic->set_app_size(800, 450);
        graphic->set_app_title("Hanoi Visualizer");
        graphic->init_window();

        while (! WindowShouldClose()) {
            BeginDrawing();
                graphic->fit_window();
                ClearBackground(RAYWHITE);

                custom_update_system->update();
                disable_system->update();
                clock_system->update();

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
                clock_render_system->update();

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