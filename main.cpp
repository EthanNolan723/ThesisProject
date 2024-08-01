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
    testRays.push_back(Ray(Vector2(500.0, 500.0), Vector2(0.0, 1.0)));
    testRays.push_back(Ray(Vector2(500.0, 500.0), Vector2(1.0, 1.0)));

    for (const Ray& ray : testRays){
        cout << 180 * refractedAngle(1, 1.1, ray, Vector2(0,1)).angle() / PI << " ";
        cout << 180 * refractedAngle(1, 1.1, ray, Vector2(-1,-1)).angle() / PI << " ";
        cout << 180 * refractedAngle(1, 1.1, ray, Vector2(1,1)).angle() / PI << "\n";
    }

    // vector<Ray> RAYS;
    // for (int i = 0; i < 256; i++){ // Generating Random Rays
    //     RAYS.push_back(Ray(Vector2(500, 500), RndDir()));
    // }
    //cout << refractedAngle(1.5, 1, 40*PI/180);

    srand (time(NULL));
    
    vector<Layer> testLayers;
    testLayers.push_back(Layer(1.5, { Vector2(100, 600), Vector2(900, 600)}));
    testLayers.push_back(Layer(1.5, { Vector2(400, 800), Vector2(600, 600)}));

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

    if (window == nullptr) {
        cerr << "Failed to create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
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
            ray.draw(renderer, 100);
        }
        //Draw Layers
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const Layer& layer : testLayers){
            SDL_RenderDrawLine(renderer, static_cast<int>(layer.getPoints()[0].x), static_cast<int>(layer.getPoints()[0].y),
                               static_cast<int>(layer.getPoints()[1].x), static_cast<int>(layer.getPoints()[1].y));
        }
        
        SDL_RenderPresent(renderer);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    cout << "\nend\n";

    string test;
    cin >> test;
    return 0;
}