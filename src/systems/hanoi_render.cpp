#include "comps/hanoi.hpp"
#include "core/ecs.hpp"
#include "utils/hanoi.hpp"
#include "services/graphic.hpp"
#include "systems/hanoi_render.hpp"

void HanoiRenderSystem::update() {
    auto graphic = ecs.get_service<GraphicService>();

    for (auto $hanoi : entities()) {
        auto hanoi = ecs.get_comp<HanoiComp>($hanoi);
        auto hanoi_bound = ecs.get_comp<BoundComp>($hanoi);
        auto x = hanoi_bound->pos.x + 0.5f * ROD_WIDTH;
        auto y1 = hanoi_bound->pos.y;
        auto y2 = y1 + (hanoi -> disk_count) * DISK_HEIGHT;
        for (RodId i = 0; i < 3; i ++) {
            graphic->draw_line({ x, y1 }, { x, y2 }, BLACK);
            x += (ROD_WIDTH + ROD_GAP);
        }
    }
}