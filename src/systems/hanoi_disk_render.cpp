#include "systems/hanoi_disk_render.hpp"
#include "comps/hanoi_disk.hpp"
#include "comps/box.hpp"
#include "services/graphic.hpp"

void HanoiDiskRenderSystem::update() {
    auto graphic = ecs.get_service<GraphicService>();

    for (auto $disk : entities()) {
        auto disk = ecs.get_comp<HanoiDiskComp>($disk);
        auto bound = ecs.get_comp<BoundComp>($disk);

        auto rec = bound->get_rect();
        graphic->fill_rect(rec, WHITE);
        Color color = disk->is_phantom
            ? LIGHTGRAY
            : disk->is_highlighted ? BLUE : BLACK;
        graphic->stroke_rect(rec, color, 1);

        if (disk->is_phantom) continue;
        const auto& id_str = disk->id_str;
        int text_width = graphic->measure_text_0(id_str, 20);
        graphic->draw_text(
            id_str,
            bound->pos + Vector2 { (bound->size.width - text_width) / 2, 1 },
            color, 20
        );
    }
}