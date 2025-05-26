#include <raylib.h>
#include "button.hpp"

void Button::update_size() {
    int text_width = MeasureText(text_.c_str(), font_size_);
    bound_.height = font_size_ + padding_ * 2;
    if (preserved_width_) {
        bound_.width = preserved_width_;
        space_padding_ = (preserved_width_ - text_width - padding_) * 0.5;
    }
    else {
        bound_.width = text_width + padding_ * 2;
    }
}

Button Button::build() {
    builded_ = true;
    update_size();
    return std::move(*this);
}

void Button::draw() const {
    DrawRectangleLinesEx(bound_, 1.f, color_);
    DrawText(
        text_.c_str(),
        bound_.x + padding_ + space_padding_,
        bound_.y + padding_,
        font_size_,
        color_
    );
}

void Button::handle_click(const Vector2& mousePos) {
    if (is_disabled_) return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, bound_)) {
            on_click_handler(*this);
        }
    }
}