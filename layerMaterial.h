#ifndef LAYERMATERIAL_H
#define LAYERMATERIAL_H

#include <cmath>
#include "Vector2.h"

using namespace std;

enum layerType {
    Refractive,
    Reflective,
    Outcoupled,
    LeftWall,
    RightWall
};

struct Layer{
private:
    vector<Vector2> points;
    enum layerType type;
    // reflection
    // flip sides (left right for inf size)
    // absorption

public:
    // Constructors
    Layer() : points(), type(Refractive) {}
    Layer( const vector<Vector2>& pts, layerType layerType)
        : points(pts), type(layerType) {}

    // Accessors
    const vector<Vector2>& getPoints() const { return points; }
    layerType getLayerType() const { return type; }

    // Method to add a point
    void addPoint(const Vector2& point) {
        points.push_back(point);
    }

    void draw(SDL_Renderer* renderer) const {
        // Draw lines between consecutive points
        for (size_t i = 0; i < points.size() - 1; ++i) {
            SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
        }
    }
};

#endif