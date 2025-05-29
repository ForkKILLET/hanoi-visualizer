#include "systems/hanoi_disk_render.hpp"
#include "comps/hanoi_disk.hpp"
#include "comps/transform.hpp"
#include "raylib.h"
#include <string>

void HanoiDiskRenderSystem::update() {
    for (auto $disk : entities()) {
        auto disk = ecs.get_comp<HanoiDiskComp>($disk);
        auto bound = ecs.get_comp<BoundComp>($disk);

        auto rec = bound->to_rec();
        DrawRectangleRec(rec, WHITE);
        DrawRectangleLinesEx(rec, 1, BLACK);

        auto id_c_str = disk->id_str.c_str();
        int text_width = MeasureText(id_c_str, 20);
        DrawText(
            id_c_str,
            bound->pos.x + (bound->size.width - text_width) / 2,
            bound->pos.y + 1,
            20, BLACK
        );
    }
}