#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

using DiskId = uint8_t;
using RodId = uint8_t;

static constexpr float ROD_WIDTH = 200.f;
static constexpr float DISK_HEIGHT = 20.f;
static constexpr float ROD_GAP = 10.f;

class HanoiSolution {
public:
    struct Step {
        RodId from;
        RodId to;
    };

    std::vector<Step> steps;
    std::optional<const Step> current_step() const {
        if (cursor_ >= steps.size()) return {};
        return { steps[cursor_] };
    }
    const size_t& cursor() const { return cursor_; }
    void next() {
        if (cursor_ < steps.size()) ++ cursor_;
    }
    void prev() {
        if (cursor_ > 0) -- cursor_;
    }

    HanoiSolution static solve(size_t disk_count) {
        HanoiSolution solution {};

        std::function<void (DiskId, DiskId, DiskId, DiskId)> solve_ = [&](DiskId from, DiskId to, DiskId aux, DiskId count) {
            if (count == 1) {
                solution.steps.push_back({from, to});
                return;
            }
            solve_(from, aux, to, count - 1);
            solution.steps.push_back({from, to});
            solve_(aux, to, from, count - 1);
        };

        solve_(0, 2, 1, disk_count);

        return solution;
    }

private:
    size_t cursor_ = 0;
};