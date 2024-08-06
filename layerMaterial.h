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
    // flip sides (left right for inf size)
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

    void draw(SDL_Renderer* renderer) const {
        // Set the color for drawing (here, we choose white, but you can change it)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Draw lines between consecutive points
        for (size_t i = 0; i < points.size() - 1; ++i) {
            SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
        }
    }
};

#endif