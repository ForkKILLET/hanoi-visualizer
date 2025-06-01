#pragma once
#include <algorithm>
#include <string>
#include <raylib.h>
#include "core/ecs.hpp"
#include "utils/vector.hpp"

using namespace vec;

class GraphicService : public Service {
public:
    GraphicService() = default;

    void draw_line(const Vector2& start, const Vector2& end, Color color, float width = 1) {
        DrawLineEx(transform(start), transform(end), transform(width), color);
    }

    void stroke_rect(const Rectangle& rect, const Color& color, float width = 1) {
        DrawRectangleLinesEx(transform(rect), transform(width), color);
    }

    void fill_rect(const Rectangle& rect, const Color& color) {
        DrawRectangleRec(transform(rect), color);
    }

    int measure_text_0(const std::string& text, int font_size) {
        return MeasureText(text.c_str(), font_size);
    }

    void draw_text(const std::string& text, const Vector2& pos, Color color, int font_size) {
        auto [ x, y ] = transform(pos);
        DrawText(text.c_str(), x, y, transform(font_size), color);
    }

    void set_app_size(int width, int height) {
        if (width <= 0 || height <= 0) throw std::invalid_argument("App size must be positive.");
        app_width = width;
        app_height = height;
    }

    void set_app_title(const std::string& title) {
        if (title.empty()) throw std::invalid_argument("App title cannot be empty.");
        app_title = title;
        if (window_inited) SetWindowTitle(title.c_str());
    }

    void init_window() {
        if (! app_width || ! app_height) throw std::runtime_error("App size is not set.");
        InitWindow(app_width, app_height, app_title.c_str());
    }

    void fit_window() {
        int real_width = GetScreenWidth();
        int real_height = GetScreenHeight();
        
        scale = std::max({
            static_cast<float>(std::min({ real_width / app_width, real_height / app_height })),
            1.0f
        });

        offset = {
            (real_width - transform(app_width)) / 2.0f,
            (real_height - transform(app_height)) / 2.0f
        };
    }

    Vector2 get_mouse_pos() {
        return untransform(GetMousePosition());
    }

    void set_cursor(int cursor) {
        if (this->cursor == cursor) return;
        this->cursor = cursor;
        SetMouseCursor(cursor);
    }

private:
    float scale { 1.0f };
    Vector2 offset { 0.0f, 0.0f };
    int cursor { MOUSE_CURSOR_DEFAULT };

    int app_width {};
    int app_height {};
    std::string app_title;

    bool window_inited { false };

    float transform(float value) {
        return value * scale;
    }
    Vector2 transform(Vector2 point) {
        return point * scale + offset;
    }
    Rectangle transform(Rectangle rect) {
        return {
            rect.x * scale + offset.x,
            rect.y * scale + offset.y,
            rect.width * scale,
            rect.height * scale
        };
    }

    Vector2 untransform(Vector2 point) {
        return (point - offset) / scale;
    }
};