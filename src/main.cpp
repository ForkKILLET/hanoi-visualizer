#include <format>
#include <raylib.h>
#include <vector>
#include "button.hpp"
#include "hanoi.hpp"
#include "timer.hpp"
#include "text.hpp"

int main() {
    InitWindow(800, 450, "Hanoi Visualizer");

    bool is_playing = false;

    auto start_button = Button()
        .set_text("Start")
        .preserve_width(90)
        .set_color(BLACK)
        .set_padding(5)
        .set_font_size(20)
        .set_position({ 5, 5 })
        .on_click([&] (Button& button) {
            is_playing = ! is_playing;
            if (is_playing) button.set_text("Pause");
            else button.set_text("Resume");
        })
        .build();

    auto hanoi = Hanoi()
        .set_disk_count(5)
        .set_position({ 5, 50 })
        .build();

    auto solution = hanoi.solve();

    auto hanoi_text = Text()
        .set_position({100, 10});

    auto hanoi_step_timer = Timer([&] {
        if (! is_playing) return;

        auto step = solution.current_step();
        if (! step) return;

        hanoi.apply_step(*step);
        solution.next();

        size_t cursor = solution.cursor();
        hanoi_text.set_text(std::format("Step: {}/{}", cursor + 1, solution.steps.size()));
    }, 1.f);


    std::vector<Entity*> entities = {
        &start_button, &hanoi, &hanoi_step_timer, &hanoi_text
    };

    Vector2 mousePos {};
    while (! WindowShouldClose()) {
        mousePos = GetMousePosition();

        for (auto entity : entities) {
            entity->update();
            entity->handle_click(mousePos);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (auto entity : entities) {
                entity->draw();
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}