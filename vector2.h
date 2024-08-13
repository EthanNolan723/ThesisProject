#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
using namespace std;

class Vector2 {
public:
    double x, y;

    // Constructors
    Vector2() : x(0), y(0) {}
    Vector2(double x, double y) : x(x), y(y) {}

    // Vector operations
    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
    Vector2 operator-() const { return Vector2(-x, -y); }
    Vector2 operator*(double t) const { return Vector2(x * t, y * t); }

    Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
    Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }

    double dot(const Vector2& v) const { return x * v.x + y * v.y; }
    double cross(const Vector2& v) const { return x * v.y - y * v.x; }

    double length() const { return sqrt(x * x + y * y); }
    double angle() const { return atan2(y, x); }

    Vector2 normalized() const {
        double len = length();
        return Vector2(x / len, y / len);
    }
};

#endif