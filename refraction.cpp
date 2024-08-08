#include "refraction.h"

Vector2 refractedAngle(float n1, float n2, Ray ray, Vector2 normal){
    
    double dotProduct = ray.getDirection().dot(normal);
    // Ensure the normal is pointing in the correct direction
    if (dotProduct == 1 || dotProduct == -1) // Pointing exact same direction
        return ray.getDirection();
    else if (dotProduct < 0){ // Correct Normal Direction
        normal = -normal;
        dotProduct = ray.getDirection().dot(normal);
    }

    float angleWithNormal = acos(dotProduct / (ray.getDirection().length() * normal.length()));

    float refractedAngle;
    if ((n1 > n2) && (angleWithNormal > asin(n2 / n1))){ // Reflection
        if (ray.getDirection().cross(normal) > 0)
            refractedAngle = normal.angle() + PI + angleWithNormal;
        else
            refractedAngle = normal.angle() + PI - angleWithNormal;
        return Vector2(cos(refractedAngle),sin(refractedAngle));
    }   
    else
        refractedAngle = asin(n1 / n2 * sin(angleWithNormal));

    // Converting to Global Co-ords
    if (ray.getDirection().cross(normal) > 0) 
        refractedAngle = normal.angle() - refractedAngle;
    else
        refractedAngle =  normal.angle() + refractedAngle; 
    
    return Vector2(cos(refractedAngle),sin(refractedAngle)); // return global angle vect
}
