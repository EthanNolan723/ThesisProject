#ifndef REFRACTEDANGLE_H
#define REFRACTEDANGLE_H

#include <cmath>
#include "Vector2.h"
#include "ray.h"

#define PI acos(-1)
using namespace std;

enum InteractionType {
    REFRACTION,
    REFLECTION
};

struct RefractedRay {
    Vector2 direction;
    InteractionType type;
};

RefractedRay refractedAngle(float n1, float n2, Ray ray, Vector2 normal);

Vector2 Reflection(const Ray& ray, const Vector2& normal);

#endif