#pragma once
#include <algorithm>
#include <cmath>
#include <compare>
#include <ostream>
#include <raylib.h>

using ord = std::partial_ordering;

class WindowTransform {
public:
    WindowTransform() = default;

    static const float& scale_x() { return sx; }
    static const float& scale_y() { return sy; }
    static const float& offset_x() { return dx; }
    static const float& offset_y() { return dy; }
    static const int scaled_window_width() { return window_width * sx; }
    static const int scaled_window_height() { return window_height * sy; }

    static Vector2 read_window_size() {
        return {
            static_cast<float>(GetScreenWidth()),
            static_cast<float>(GetScreenHeight())
        };
    }

    static void write_window_size() {
        SetWindowSize(scaled_window_width(), scaled_window_height());
    }

    static void set_window_size(int width, int height) {
        window_width = width;
        window_height = height;
        write_window_size();
    }

    static void set_scale(float s) {
        sx = sy = std::max({ s, 1.0f });
    }

    static void set_scale(Vector2 s) {
        sx = std::max({ s.x, 1.0f });
        sy = std::max({ s.y, 1.0f });
    }

    static void fit_window_size() {
        Vector2 size = read_window_size();
        set_scale(std::min({ std::floor(size.x / window_width), std::floor(size.y / window_height) }));
        dx = (size.x - scaled_window_width()) / 2;
        dy = (size.y - scaled_window_height()) / 2;
    }

private:
    static int window_width;
    static int window_height;

    static float sx;
    static float sy;
    static float dx;
    static float dy;
};

inline int WindowTransform::window_width;
inline int WindowTransform::window_height;
inline float WindowTransform::sx = 1;
inline float WindowTransform::sy = 1;
inline float WindowTransform::dx = 0;
inline float WindowTransform::dy = 0;

class VV {
public:
    constexpr VV() = default;
    constexpr VV(float x) : v(x) {}
    static VV from_real(float rv) { return rv / WindowTransform::scale_x(); }

    float v;

    float to_real() const { return v * WindowTransform::scale_x(); }

    operator float() const { return to_real(); }

    constexpr VV operator+(const VV& other) const { return { v + other.v }; }
    constexpr VV operator-(const VV& other) const { return { v - other.v }; }
    constexpr VV operator*(const VV& other) const { return { v * other.v }; }

    constexpr VV operator*(float m) const { return { v * m }; }
    constexpr VV operator*(int m) const { return { v * m }; }
    constexpr VV operator/(float m) const { return { v / m }; }
    constexpr VV operator/(int m) const { return { v / m }; }

    VV& operator+=(const VV& other) { v += other.v; return *this; }
    VV& operator-=(const VV& other) { v -= other.v; return *this; }
    VV& operator*=(const VV& other) { v *= other.v; return *this; }
    VV& operator*=(float m) { v *= m; return *this; }
};

inline VV operator "" _v (long double value) { return static_cast<float>(value); }
inline VV operator "" _v (unsigned long long value) { return static_cast<float>(value); }

using VV1 = VV;

class VV2 {
public:
    constexpr VV2() = default;
    constexpr VV2(VV x, VV y) : x(x), y(y) {}
    static VV2 from_real(Vector2 rv) { return { rv.x / WindowTransform::scale_x(), rv.y / WindowTransform::scale_y() }; }

    VV x;
    VV y;

    Vector2 to_real() const { return { x, y }; }

    constexpr VV2 operator+(const VV2& other) const { return { x + other.x, y + other.y }; }
    constexpr VV2 operator-(const VV2& other) const { return { x - other.x, y - other.y }; }
    constexpr VV2 operator*(const VV2& other) const { return { x * other.x, y * other.y }; }

    constexpr VV2 operator*(const Vector2& other) const { return { x * other.x, y * other.y }; }

    constexpr VV2 operator*(float m) const { return { x * m, y * m }; }
    constexpr VV2 operator/(float m) const { return { x / m, y / m }; }

    constexpr VV magnitude() const { return sqrtf(x.v * x.v + y.v * y.v); }
};

inline Vector2 operator+(const Vector2& a, const Vector2& b) {
    return { a.x + b.x, a.y + b.y };
}
inline Vector2 operator-(const Vector2& a, const Vector2& b) {
    return { a.x - b.x, a.y - b.y };
}
inline Vector2 operator*(const Vector2& a, float m) {
    return { a.x * m, a.y * m };
}
inline Vector2 operator*(float m, const Vector2& a) {
    return { a.x * m, a.y * m };
}
inline Vector2 operator*(const Vector2& a, const Vector2& b) {
    return { a.x * b.x, a.y * b.y };
}

inline ord operator||(ord a, ord b) {
    if (a == ord::equivalent) return b;
    return a;
}
inline ord operator<=>(const Vector2& a, const Vector2& b) {
    return a.x <=> b.x || a.y <=> b.y;
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

template <typename V>
requires requires(V v) {
    { v.magnitude() };
}
inline auto magnitude(V v) {
    return v.magnitude();
}

template <typename = Vector2>
inline float magnitude(const Vector2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}
