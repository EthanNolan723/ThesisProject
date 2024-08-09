#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <algorithm>
#include "refraction.h"
#include "ray.h"
#include "Vector2.h"
#include "layerMaterial.h"

#include <SDL2/SDL.h>

using namespace std;

#define SDL_WINDOW_WIDTH 1000
#define SDL_WINDOW_HEIGHT 1000

Vector2 RndDir(){ // Temp
    double randAngle = 2*PI * rand() / (RAND_MAX + 1);
    return Vector2(sin(randAngle), cos(randAngle));
}

int WinMain(int argc, char* argv[]){
    vector<Ray> testRays;
    testRays.push_back(Ray(Vector2(500.0, 500.0), Vector2(0.0, 1.0), 0));
    testRays.push_back(Ray(Vector2(500.0, 500.0), Vector2(1.0, 1.0), 0));
    testRays.push_back(Ray(Vector2(500.0, 500.0), Vector2(-1.0, 1.0), 0));
    testRays.push_back(Ray(Vector2(600.0, 900.0), Vector2(0.0, -1.0), 0));

    vector<Layer> testLayers;
    testLayers.push_back(Layer(1.1, { Vector2(100, 600), Vector2(900, 600)}));
    testLayers.push_back(Layer(1.1, { Vector2(100, 700), Vector2(900, 700)}));
    testLayers.push_back(Layer(1.1, { Vector2(900, 700), Vector2(100, 900)}));

    for (const Ray& ray : testRays){ // Testing Refraction
        cout << 180 * refractedAngle(1, 1.1, ray, Vector2(0,1)).angle() / PI << " ";
        cout << 180 * refractedAngle(1, 1.1, ray, Vector2(-1,-1).normalized()).angle() / PI << " ";
        cout << 180 * refractedAngle(1, 1.1, ray, Vector2(1,1).normalized()).angle() / PI << "\n";
    } // 90 85.0027 85.0027 /n 49.9973 45 45
    
    size_t i = 0;
    while (i < testRays.size()){
        Ray& ray = testRays[i];
        if (ray.getBounces() < 6){
            HitInfo closestHit;
            closestHit.didHit = false;
            for (const Layer& layer : testLayers){
                HitInfo hit = collisionDetectionGOOGOOGAGA(ray, layer.getPoints());
                if (!closestHit.didHit || hit.distance < closestHit.distance){
                    closestHit = hit;
                }
            }
            // TODO correct the refractive index changes
            if (closestHit.didHit){
                Vector2 newAngle = refractedAngle(2, 1.1, ray, closestHit.normal);
                testRays.push_back(Ray(closestHit.hitPoint, newAngle, ray.getBounces()+1));
            }
        }
        ++i;
    }

    //Create Empty Window
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",                    // Window title
        SDL_WINDOWPOS_CENTERED,           // Initial x position
        SDL_WINDOWPOS_CENTERED,           // Initial y position
        SDL_WINDOW_WIDTH,                 // Width in pixels
        SDL_WINDOW_HEIGHT,                // Height in pixels
        SDL_WINDOW_SHOWN                  // Flags
    );
    if (window == nullptr) { // Window Doesn't Open
        cerr << "Failed to create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) { // Renderer Fails
        cerr << "Failed to create renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Main event loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw Rays
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        for (const Ray& ray : testRays) {
            ray.draw(renderer, 75);
        }
        //Draw Layers
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const Layer& layer : testLayers){
            layer.draw(renderer);
        }
        
        SDL_RenderPresent(renderer);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    cout << "\nend\n";
    string test; // Leave consol open
    cin >> test;
    return 0;
}

// ** RANDOM RAYS
// vector<Ray> RAYS; 
    // for (int i = 0; i < 256; i++){ // Generating Random Rays
    //     RAYS.push_back(Ray(Vector2(500, 500), RndDir()));
    // }
    // cout << refractedAngle(1.5, 1, 40*PI/180);

    // srand (time(NULL));