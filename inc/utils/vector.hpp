#pragma once
#include <cmath>
#include <compare>
#include <ostream>
#include <raylib.h>

namespace vec {

inline Vector2 operator+(const Vector2& a, const Vector2& b) {
    return { a.x + b.x, a.y + b.y };
}
inline Vector2 operator-(const Vector2& a, const Vector2& b) {
    return { a.x - b.x, a.y - b.y };
}
inline Vector2 operator*(const Vector2& a, float k) {
    return { a.x * k, a.y * k };
}
inline Vector2 operator*(float k, const Vector2& a) {
    return { k * a.x, k * a.y };
}
inline Vector2 operator/(const Vector2& a, float k) {
    return { a.x / k, a.y / k };
}
inline Vector2 operator*(const Vector2& a, const Vector2& b) {
    return { a.x * b.x, a.y * b.y };
}

using ord = std::partial_ordering;
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

inline float vec_length(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

template<typename T>
auto unpack(const Vector2& v) {
    return std::tie(v.x, v.y);
}

};