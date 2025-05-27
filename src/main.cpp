#include <raylib.h>
#include "ecs.hpp"
#include "entities/text.hpp"
#include "comps/text.hpp"
#include "comps/transform.hpp"
#include "comps/clickable.hpp"
#include "systems/click.hpp"
#include "systems/text_render.hpp"
#include "systems/anchor.hpp"
#include "systems/text_typesetting.hpp"

class App {
public:
    ECS ecs {};

    void start() {
        ecs.register_comp<TextComp>();
        ecs.register_comp<BoundComp>();
        ecs.register_comp<AnchorComp>();
        ecs.register_comp<ClickableComp>();

        auto anchor_system = ecs.register_system<AnchorSystem>()
            ->with_comp<AnchorComp>()
            ->with_comp<BoundComp>();
        auto text_typesetting_system = ecs.register_system<TextTypesettingSystem>()
            ->with_comp<TextComp>()
            ->with_comp<BoundComp>();
        auto text_render_system = ecs.register_system<TextRenderSystem>()
            ->with_comp<TextComp>()
            ->with_comp<BoundComp>();
        auto click_system = ecs.register_system<ClickSystem>()
            ->with_comp<ClickableComp>()
            ->with_comp<BoundComp>();

        auto title_text = create_text(ecs)
            .text("Hanoi Visualizer")
            .color(BLACK)
            .font_size(20)
            .anchor({ 400, 20 }, TOP_CENTER)
            .build();

        ecs.emplace_comp<ClickableComp>(title_text, Delegate<> {
            []() {
                TraceLog(LOG_INFO, "Title clicked!");
            }
        });

        InitWindow(800, 450, "Hanoi Visualizer");

        while (! WindowShouldClose()) {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                text_typesetting_system->update();
                anchor_system->update();
                text_render_system->update();
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