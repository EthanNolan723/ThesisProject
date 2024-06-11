#include <iostream>
#include <vector>
#include <string>
#include "refraction.h"
#include "ray.h"
#include "layerMaterial.h"

#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720

using namespace std;

HitInfo intersection(const Ray& ray, const layer){ // Simplification assuming that all layers are horizontal.
    // take big steps (1, 2, 3)
    // when above line take steps back (2.5)
    
}

int main(){
    float msg = refractedAngle(1.1, 1, 40*PI/180) * 180 / (PI);
    cout << msg << " ";

    Ray testRay(Vector2(1, 2), Vector2(0, 1)); // vector at 1, 2 pointing directly up
    layer TestLayer(1, [](double x) {return 4; }); // layer with refractive index 1 at y=4;



    return 0;
}