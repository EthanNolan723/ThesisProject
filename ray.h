#ifndef RAY_H
#define RAY_H

#include "Vector2.h"

class Ray {
private:
    Vector2 position;
    Vector2 direction;
public:
    // Constructors
    Ray() : position(0.0, 0.0), direction(0.0, 0.0) {}
    Ray(const Vector2& origin, const Vector2& direction) : position(origin), direction(direction.normalized()) {}

    // Accessors
    Vector2 getPos() const { return position; }
    Vector2 getDirection() const { return direction; }

    // Method to get a point along the ray at parameter t
    Vector2 pointAtParameter(double t) const {
        return position + direction * t;
    }
};

#endif // RAY_H