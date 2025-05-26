#include <compare>
#include <ostream>
#include <raylib.h>

inline Vector2 operator+(const Vector2& a, const Vector2& b) {
    return {a.x + b.x, a.y + b.y};
}
inline Vector2 operator-(const Vector2& a, const Vector2& b) {
    return {a.x - b.x, a.y - b.y};
}
inline Vector2 operator*(const Vector2& a, float k) {
    return {a.x * k, a.y * k};
}
inline Vector2 operator*(float k, const Vector2& a) {
    return {a.x * k, a.y * k};
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