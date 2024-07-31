#ifndef REFRACTEDANGLE_H
#define REFRACTEDANGLE_H

#include <cmath>
#include "Vector2.h"
#include "ray2.h"

#define PI acos(-1)

float refractedAngle(float n1, float n2, ray Ray, Vector2 normal);

#endif