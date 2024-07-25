#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "refraction.h"
#include "ray.h"
#include "layerMaterial.h"

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720

using namespace std;

// HitInfo intersection(const Ray& ray, const layer){ // Simplification assuming that all layers are horizontal.
//     // take big steps (1, 2, 3)
//     // when above line take steps back (2.5)
    
// }

int WinMain(int argc, char* argv[]){
    float msg = refractedAngle(1.1, 1, 40*PI/180) * 180 / (PI);
    cout << msg << " ";

    Ray testRay(Vector2(1, 2), Vector2(0, 1)); // vector at 1, 2 pointing directly up
    layer TestLayer(1, [](double x) {return 4; }); // layer with refractive index 1 at y=4;




    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",                    // Window title
        SDL_WINDOWPOS_CENTERED,           // Initial x position
        SDL_WINDOWPOS_CENTERED,           // Initial y position
        800,                              // Width in pixels
        600,                              // Height in pixels
        SDL_WINDOW_SHOWN                  // Flags
    );

    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
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
    }

    // Clean up and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();




    cout << "\nend\n";

    string test;
    cin >> test;
    return 0;
}