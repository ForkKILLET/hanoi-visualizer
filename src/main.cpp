#include <raylib.h>
#include "comps/hanoi.hpp"
#include "comps/hanoi_disk.hpp"
#include "core/ecs.hpp"
#include "entities/text.hpp"
#include "entities/hanoi.hpp"
#include "comps/text.hpp"
#include "comps/transform.hpp"
#include "comps/clickable.hpp"
#include "systems/animation.hpp"
#include "systems/click.hpp"
#include "systems/hanoi.hpp"
#include "systems/hanoi_disk_render.hpp"
#include "systems/text_render.hpp"
#include "systems/anchor.hpp"
#include "systems/text_typesetting.hpp"
#include "systems/timer.hpp"
#include "systems/hanoi_render.hpp"

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

        auto anchor_system = ecs.register_system<AnchorSystem>();
        auto text_typesetting_system = ecs.register_system<TextTypesettingSystem>();
        auto text_render_system = ecs.register_system<TextRenderSystem>();
        auto hanoi_system = ecs.register_system<HanoiSystem>();
        auto hanoi_render_system = ecs.register_system<HanoiRenderSystem>();
        auto hanoi_disk_render_system = ecs.register_system<HanoiDiskRenderSystem>();
        auto timer_system = ecs.register_system<TimerSystem>();
        auto click_system = ecs.register_system<ClickSystem>();
        auto animation_system = ecs.register_system<AnimationSystem>();

        auto $title_text = TextBuilder(ecs)
            .text("Hanoi Visualizer")
            .color(BLACK)
            .font_size(20)
            .anchor({ 400, 20 }, TOP_CENTER)
            .build();

        auto $hanoi = HanoiBuilder(ecs)
            .disk_count(5)
            .pos({ 20, 40 })
            .build();

        ecs.emplace_comp<ClickableComp>($title_text, Delegate<> {
            []() {
                TraceLog(LOG_INFO, "Title clicked!");
            }
        });

        InitWindow(800, 450, "Hanoi Visualizer");

        while (! WindowShouldClose()) {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                animation_system->update();

                text_typesetting_system->update();
                anchor_system->update();

                hanoi_system->update();

                text_render_system->update();
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