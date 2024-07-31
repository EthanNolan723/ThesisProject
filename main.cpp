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

Vector2 RndDir(){ // Temp
    double randAngle = 2*PI * rand() / (RAND_MAX + 1);
    return Vector2(sin(randAngle), cos(randAngle));
}

int WinMain(int argc, char* argv[]){
    float msg = refractedAngle(1.1, 1, 40*PI/180) * 180 / (PI);
    cout << msg << " ";
    srand (time(NULL));
    
    // vector<Ray> RAYS;
    // for (int i = 0; i < 256; i++){ // Generating Random Rays
    //     RAYS.push_back(Ray(Vector2(500, 500), RndDir()));
    // }

    Ray testRay(Vector2(500, 500), Vector2(0,1)); // Ray in center pointing straight up (down on screen)

    Layer TestLayer(1.5, { Vector2(100, 600), Vector2(900, 600)}); // layer with refractive index 1 at y=4;

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
        1000,                              // Width in pixels
        1000,                              // Height in pixels
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

        // int loopColour = 0;
        // // Draw the ray
        // for (const Ray& ray : RAYS) {
        //     SDL_SetRenderDrawColor(renderer,
        //                            max(min(255,255*2-loopColour),0), 
        //                            min(min(255,loopColour),(255*3-loopColour)),
        //                            min(max(0,loopColour-255*2),255),
        //                            255);
        //     ray.draw(renderer, 100);
        //     loopColour += 4;
        // }
        // // Update screen
        // SDL_RenderPresent(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        testRay.draw(renderer,400);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, static_cast<int>(TestLayer.getPoints()[0].x), static_cast<int>(TestLayer.getPoints()[0].y),static_cast<int>(TestLayer.getPoints()[1].x), static_cast<int>(TestLayer.getPoints()[1].y));
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