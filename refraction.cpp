#include "refraction.h"

Vector2 refractedAngle(float n1, float n2, Ray ray, Vector2 normal){
    // Ensure the normal is pointing in the correct direction
    if (ray.getDirection().dot(normal) < 0) {
        normal = -normal;
    }
    
    float angleWithNormal = acos(ray.getDirection().dot(normal) / (ray.getDirection().length() * normal.length()));

    float refractedAngleWithNormal;
    if ((n1 > n2) && (angleWithNormal > asin(n2 / n1))) {
        refractedAngleWithNormal = -angleWithNormal;
    } else {
        refractedAngleWithNormal = asin(n1 / n2 * sin(angleWithNormal));
    }

    // Convert the refracted angle back to the global coordinate system
    float globalRefractedAngle = normal.angle() - refractedAngleWithNormal;

    return Vector2(cos(globalRefractedAngle),sin(globalRefractedAngle)); // return global angle
}
