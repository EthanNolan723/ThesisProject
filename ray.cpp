#include "ray.h"
#include <cmath>

HitInfo collisionDetectionGOOGOOGAGA(const Ray& ray, const vector<Vector2>& points) {
    HitInfo closestHit;
    closestHit.didHit = false;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        Vector2 lineDir = points[i+1] - points[i];

        // Calculate the determinants
        double det = lineDir.cross(ray.getDirection());
        if (abs(det) < 1e-8) { // If det is close to zero, lines are parallel
            continue;
        }

        Vector2 diff = ray.getPos() - points[i];
        double t = diff.cross(lineDir) / det;
        double u = diff.cross(ray.getDirection()) / det;

        // Check if the intersection occurs within the bounds of the ray and the line segment
        if (t > 1e-8 && u >= 0 && u <= 1) {
            Vector2 hitPoint = ray.getPos() + ray.getDirection() * t;
            Vector2 lineNormal = Vector2(-lineDir.y, lineDir.x).normalized(); // Perpendicular to the line

            if (!closestHit.didHit || t < closestHit.distance) {
                closestHit.didHit = true;
                closestHit.distance = t;
                closestHit.hitPoint = hitPoint;
                closestHit.normal = lineNormal;
            }
        }
    }
    return closestHit;
}