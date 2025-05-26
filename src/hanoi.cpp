#include <cassert>
#include <cstddef>
#include <functional>
#include <raylib.h>
#include <string>
#include "hanoi.hpp"
#include "utils.hpp"

Hanoi Hanoi::set_disk_count(size_t disk_count) {
    disk_count_ = disk_count;
    for (auto rod : rods) {
        rod.clear();
        rod.reserve(disk_count);
    }
    for (size_t i = disk_count; i > 0; --i) {
        rods[0].push_back(i);
    }
    return *this;
}

Hanoi Hanoi::build() {
    return std::move(*this);
}

Hanoi::Solution Hanoi::solve() const {
    Solution solution {};

    std::function<void (size_t, size_t, size_t, size_t)> solve_ = [&](size_t from, size_t to, size_t aux, size_t count) {
        if (count == 1) {
            solution.steps.push_back({from, to});
            return;
        }
        solve_(from, aux, to, count - 1);
        solution.steps.push_back({from, to});
        solve_(aux, to, from, count - 1);
    };

    solve_(0, 2, 1, rods[0].size());

    return solution;
}

void Hanoi::apply_step(const SolutionStep& step) {
    auto& from_rod = rods[step.from];
    auto& to_rod = rods[step.to];
    assert(! from_rod.empty());
    size_t disk_id = from_rod.back();
    from_rod.pop_back();
    to_rod.push_back(disk_id);
}

void Hanoi::unapply_step(const SolutionStep& step) {
    apply_step({step.to, step.from});
}

constexpr float rod_width = 200.f;
constexpr float disk_height = 20.f;
constexpr float rod_gap = 10.f;

void Hanoi::draw() const {
    const float rod_height = disk_height * disk_count_;

    float y0 = pos_.y + rod_height;
    const float disk_width_step = rod_width / disk_count_;

    for (size_t i = 0; i < 3; ++ i) {
        const auto& rod = rods[i];

        float y = y0;
        float x_mid = pos_.x + rod_width * (i + 0.5) + rod_gap * i;

        size_t disk_count = rod.size();
        for (size_t j = 0; j < disk_count; ++ j) {
            size_t disk_id = rod[j];
            y -= disk_height;
            float w = disk_width_step * disk_id;
            DrawRectangleLines(x_mid - w / 2, y, w, disk_height, BLACK);
            std::string disk_id_str = std::to_string(disk_id);
            float text_width = MeasureText(disk_id_str.c_str(), 20);
            DrawText(disk_id_str.c_str(), x_mid - text_width / 2, y + disk_height / 2 - 10, 20, BLACK);
        }

        DrawLine(x_mid, pos_.y, x_mid, y, BLACK);
        DrawLine(x_mid - rod_width / 2, y0, x_mid + rod_width / 2, y0, BLACK);
    }
}
