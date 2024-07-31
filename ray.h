#ifndef RAY_H
#define RAY_H

#include "Vector2.h"
#include <SDL2/SDL.h>

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
    Vector2 step(double t) const {
        return position + direction * t;
    }

    void draw(SDL_Renderer* renderer, int distance) const {
        Vector2 endPoint = step(distance);
        SDL_RenderDrawLine(renderer, static_cast<int>(position.x), static_cast<int>(position.y),static_cast<int>(endPoint.x), static_cast<int>(endPoint.y));
    }
};

struct HitInfo{  // If ray hits a object
    bool didHit;
    float distance;
    Vector2 hitPoint;
    Vector2 normal;
};

#endif // RAY_H