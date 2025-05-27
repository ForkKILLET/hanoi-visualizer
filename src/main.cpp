#include <raylib.h>
#include "ecs.hpp"
#include "entities/text.hpp"
#include "comps/text.hpp"
#include "comps/transform.hpp"
#include "systems/text_render.hpp"

class App {
public:
    ECS ecs {};
    Vector2 mousePos {};

    void start() {
        ecs.register_comp<TextComp>();
        ecs.register_comp<PosComp>();

        auto buttonRenderSystem = ecs.register_system<TextRenderSystem>()
            ->with_comp<TextComp>()
            ->with_comp<PosComp>();

        create_text(ecs)
            .text("Hanoi Visualizer")
            .color(BLACK)
            .font_size(20)
            .pos({ 20, 20 })
            .build();

        InitWindow(800, 450, "Hanoi Visualizer");

        while (! WindowShouldClose()) {
            mousePos = GetMousePosition();

            BeginDrawing();
                ClearBackground(RAYWHITE);
                buttonRenderSystem->update();
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