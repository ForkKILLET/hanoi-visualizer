#include <raylib.h>
#include "comps/hanoi.hpp"
#include "core/ecs.hpp"
#include "utils/hanoi.hpp"
#include "systems/hanoi_render.hpp"

void HanoiRenderSystem::update() {
    for (auto $hanoi : entities()) {
        auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
        auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);
        VV x = hanoi_bound->pos.x + ROD_WIDTH * 0.5f;
        VV y1 = hanoi_bound->pos.y;
        VV y2 = y1 + DISK_HEIGHT * hanoi -> disk_count;
        for (RodId i = 0; i < 3; i ++) {
            DrawLineEx({ x, y1 }, { x, y2 }, 1_v, BLACK);
            x += ROD_WIDTH + ROD_GAP;
        }
    }
}