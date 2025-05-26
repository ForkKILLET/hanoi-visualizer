#pragma once
#include <array>
#include <cstddef>
#include <optional>
#include <vector>
#include "entity.hpp"
#include "raylib.h"

using std::size_t;

class Hanoi : public Entity {
private:
    std::array<std::vector<size_t>, 3> rods;

    Vector2 pos_;
    size_t disk_count_;

public:
    struct SolutionStep {
        size_t from;
        size_t to;
    };

    class Solution {
    private:
        size_t cursor_ = 0;

    public:
        std::vector<SolutionStep> steps;
        std::optional<const SolutionStep> current_step() const {
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
    };

    explicit Hanoi() = default;

    Hanoi set_disk_count(size_t disk_count);
    Hanoi set_position(const Vector2& pos) {
        pos_ = pos;
        return *this;
    }
    Hanoi build();

    Solution solve() const;
    void apply_step(const SolutionStep& step);
    void unapply_step(const SolutionStep& step);

    void draw() const override;
    void update() override {}
};