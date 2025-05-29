#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

using DiskId = uint8_t;
using RodId = uint8_t;

static constexpr float ROD_WIDTH = 200.f;
static constexpr float DISK_HEIGHT = 20.f;
static constexpr float ROD_GAP = 10.f;
static constexpr float HANOI_WIDTH = 3 * ROD_WIDTH + 2 * ROD_GAP;

class HanoiSolution {
public:
    struct Step {
        RodId from;
        RodId to;
    };

    std::vector<Step> steps;
    const Step& current_step() const {
        return steps[cursor_];
    }
    const size_t& cursor() const { return cursor_; }

    bool has_next() const { return cursor_ < steps.size(); }
    bool has_prev() const { return cursor_ > 0; }
    void next() {
        assert(has_next());
        ++ cursor_;
    }
    void prev() {
        assert(has_prev());
        -- cursor_;
    }

    HanoiSolution static solve(size_t disk_count) {
        HanoiSolution solution {};

        std::function<void (DiskId, DiskId, DiskId, DiskId)> solve_ = [&](DiskId from, DiskId to, DiskId aux, DiskId count) {
            if (count == 1) {
                solution.steps.push_back({ from, to });
                return;
            }
            solve_(from, aux, to, count - 1);
            solution.steps.push_back({ from, to });
            solve_(aux, to, from, count - 1);
        };

        solve_(0, 2, 1, disk_count);

        return solution;
    }

private:
    size_t cursor_ = 0;
};