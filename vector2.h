#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

class Vector2 {
public:
    double x, y;

    // Constructors
    Vector2() : x(0), y(0) {}
    Vector2(double x, double y) : x(x), y(y) {}

    // Vector operations
    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
    Vector2 operator*(double t) const { return Vector2(x * t, y * t); }

    double dot(const Vector2& v) const { return x * v.x + y * v.y; }
    double cross(const Vector2& v) const { return x * v.y - y * v.x; }

    double length() const { return std::sqrt(x * x + y * y); }

    Vector2 normalized() const {
        double len = length();
        return Vector2(x / len, y / len);
    }
};

#endif