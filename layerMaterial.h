#ifndef LAYERMATERIAL_H
#define LAYERMATERIAL_H

#include <cmath>
#include "Vector2.h"

using namespace std;

struct Layer{
    private:
    vector<Vector2> points;
    float refractiveIndex;
    // reflection
    // flip sides
    // absorption

    public:
    // Constructors
    Layer() : points(), refractiveIndex(1) {}
    Layer(float refrIndex, const vector<Vector2>& pts)
        : points(pts), refractiveIndex(refrIndex) {}

    // Accessors
    const vector<Vector2>& getPoints() const { return points; }
    float getRefInd() const { return refractiveIndex; }

    // Method to add a point
    void addPoint(const Vector2& point) {
        points.push_back(point);
    }
};

#endif