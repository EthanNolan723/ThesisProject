#include "refraction.h"

RefractedRay refractedAngle(float n1, float n2, Ray ray, Vector2 normal) {
    double dotProduct = ray.getDirection().dot(normal);

    // Ensure the normal is pointing in the correct direction
    if (dotProduct == 1 || dotProduct == -1) // Pointing in the exact same direction
        return {ray.getDirection(), REFRACTION};
    else if (dotProduct < 0) { // Correct Normal Direction
        normal = -normal;
        dotProduct = ray.getDirection().dot(normal);
    }

    double angleWithNormal = acos(dotProduct / (ray.getDirection().length() * normal.length()));

    double refractedAngle;
    if ((n1 > n2) && (angleWithNormal > asin(n2 / n1))) { // Reflection
        Vector2 reflectionDirection = Reflection(ray, normal);
        return {reflectionDirection, REFLECTION};
    }   
    else {
        refractedAngle = asin(n1 / n2 * sin(angleWithNormal));
    }

    // Converting to Global Co-ords
    if (ray.getDirection().cross(normal) > 0) 
        refractedAngle = normal.angle() - refractedAngle;
    else
        refractedAngle = normal.angle() + refractedAngle; 
    
    return {Vector2(cos(refractedAngle), sin(refractedAngle)), REFRACTION};
}

Vector2 Reflection(const Ray& ray, const Vector2& normal){
    double dotProduct = ray.getDirection().dot(normal);
    double angleWithNormal = acos(dotProduct / (ray.getDirection().length() * normal.length()));
    if (ray.getDirection().cross(normal) == 0) {
        return -ray.getDirection(); // Direct reflection
    }
    else if (ray.getDirection().cross(normal) > 0) {
        return Vector2(cos(normal.angle() + PI + angleWithNormal), sin(normal.angle() + PI + angleWithNormal));
    }
    else {
        return Vector2(cos(normal.angle() + PI - angleWithNormal), sin(normal.angle() + PI - angleWithNormal));
    }
}