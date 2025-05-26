#include "entity.hpp"
#include "raylib.h"
#include <string>

class Text : public Entity {
private:
    std::string text_ {};
    Vector2 pos_ {};
    Color color_ = BLACK;
    int font_size_ = 20;

public:
    explicit Text() = default;

    const std::string& text() { return text_; }
    Text set_text(std::string text) {
        text_ = std::move(text);
        return *this;
    }

    const Vector2& position() { return pos_; }
    Text set_position(const Vector2& pos) {
        pos_ = pos;
        return *this;
    }

    const Color& color() { return color_; };
    Text set_color(Color color) {
        color_ = color;
        return *this;
    }

    const int& font_size() { return font_size_; }
    Text set_font_size(int font_size) {
        font_size_ = font_size;
        return *this;
    }

    void draw() const override;

    void update() override {}
};