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

    float refractedAngleWithNormal;
    if ((n1 > n2) && (angleWithNormal > asin(n2 / n1))) // Total Internal Reflection
        refractedAngleWithNormal = -angleWithNormal; //TODO FIX THIS
    else
        refractedAngleWithNormal = asin(n1 / n2 * sin(angleWithNormal));

    // Converting to Global Co-ords
    float globalRefractedAngle;
    if (ray.getDirection().cross(normal) > 0) 
        globalRefractedAngle = normal.angle() - refractedAngleWithNormal;
    else
        globalRefractedAngle =  normal.angle() + refractedAngleWithNormal; 
    
    return Vector2(cos(globalRefractedAngle),sin(globalRefractedAngle)); // return global angle vect
}
