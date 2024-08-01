#ifndef REFRACTEDANGLE_H
#define REFRACTEDANGLE_H

#include <cmath>
#include "Vector2.h"
#include "ray.h"

#define PI acos(-1)
using namespace std;

Vector2 refractedAngle(float n1, float n2, Ray ray, Vector2 normal);

#endif