#pragma once
#include <string>
#include <raylib.h>
#include "entity.hpp"

class Button : public Entity {
protected:
    std::string text_ {};
    Color color_ = BLACK;
    int padding_ = 10;
    int font_size_ = 20;
    Rectangle bound_ {};
    int preserved_width_ = 0;
    float space_padding_ = 0;

    bool builded_ = false;
    bool is_disabled_ = false;
    OnClickHandler<Button> on_click_handler;

    void update_size();

public:
    explicit Button() = default;

    const std::string& text() { return text_; }
    Button set_text(std::string text) {
        text_ = std::move(text);
        if (builded_) update_size();
        return *this;
    }

    const Color& color() { return color_; };
    Button set_color(Color color) {
        color_ = color;
        return *this;
    }

    const int& padding() { return padding_; }
    Button set_padding(int padding) {
        padding_ = padding;
        return *this;
    }

    const int& font_size() { return font_size_; }
    Button set_font_size(int font_size) {
        font_size_ = font_size;
        if (builded_) update_size();
        return *this;
    }

    Button preserve_width(int width) {
        preserved_width_ = width;
        return *this;
    }

    const Rectangle& bound() { return bound_; }
    Button set_position(const Vector2& pos) {
        bound_.x = pos.x;
        bound_.y = pos.y;
        return *this;
    }

    const bool& is_disabled() { return is_disabled_; }
    Button set_disabled(bool disabled) {
        is_disabled_ = disabled;
        return *this;
    }

    Button on_click(OnClickHandler<Button> handler) {
        on_click_handler = handler;
        return *this;
    }

    Button build();

    void draw() const override;
    void handle_click(const Vector2& mousePos) override;
    void update() override {}
};
