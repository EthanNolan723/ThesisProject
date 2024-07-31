#include "refraction.h"

float refractedAngle(float n1, float n2, ray Ray, Vector2 normal);{

    float dotProduct = ray.direction.dot(normal);
    float angleWithNormal = acos(dotProduct);

    if ((n1 > n2) && (angleWithNormal > asin(n2 / n1))) {
        refractedAngleWithNormal = -angleWithNormal;
    } else {
        refractedAngleWithNormal = asin(n1 / n2 * sin(angleWithNormal));
    }

    // Convert the refracted angle back to the global coordinate system
    float globalRefractedAngle = atan2(sin(incidentAngle1) * cos(refractedAngleWithNormal) - cos(incidentAngle1) * sin(refractedAngleWithNormal), 
                                       cos(incidentAngle1) * cos(refractedAngleWithNormal) + sin(incidentAngle1) * sin(refractedAngleWithNormal));

    return globalRefractedAngle; // return global angle
}
