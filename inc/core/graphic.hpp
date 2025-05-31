#pragma once
#include <algorithm>
#include <string>
#include <raylib.h>
#include "utils/vector.hpp"

using namespace vec;

namespace graphic {

struct Transform {
    static float scale;
    static Vector2 offset;
};

inline float Transform::scale { 1.0f };
inline Vector2 Transform::offset { 0.0f, 0.0f };

inline static int app_width = 0;
inline static int app_height = 0;

inline float transform(float value) {
    return value * Transform::scale;
}
inline Vector2 transform(Vector2 point) {
    return point * Transform::scale + Transform::offset;
}
inline Rectangle transform(Rectangle rect) {
    return {
        rect.x * Transform::scale + Transform::offset.x,
        rect.y * Transform::scale + Transform::offset.y,
        rect.width * Transform::scale,
        rect.height * Transform::scale
    };
}

inline Vector2 untransform(Vector2 point) {
    return (point - Transform::offset) / Transform::scale;
}

inline void draw_line(const Vector2& start, const Vector2& end, Color color, float width = 1) {
    DrawLineEx(transform(start), transform(end), transform(width), color);
}

inline void stroke_rect(const Rectangle& rect, const Color& color, float width = 1) {
    DrawRectangleLinesEx(transform(rect), transform(width), color);
}

inline void fill_rect(const Rectangle& rect, const Color& color) {
    DrawRectangleRec(transform(rect), color);
}

inline int measure_text_0(const std::string& text, int font_size) {
    return MeasureText(text.c_str(), font_size);
}

inline void draw_text(const std::string& text, const Vector2& pos, Color color, int font_size) {
    auto [ x, y ] = transform(pos);
    DrawText(text.c_str(), x, y, transform(font_size), color);
}

inline void fit_window() {
    if (app_width == 0 || app_height == 0) {
        throw std::runtime_error("Window size is not set.");
    }

    int real_width = GetScreenWidth();
    int real_height = GetScreenHeight();
    
    Transform::scale = std::max({
        static_cast<float>(std::min({ real_width / app_width, real_height / app_height })),
        1.0f
    });

    Transform::offset = {
        (real_width - transform(app_width)) / 2.0f,
        (real_height - transform(app_height)) / 2.0f
    };
}

inline Vector2 get_mouse_pos() {
    return untransform(GetMousePosition());
}

}