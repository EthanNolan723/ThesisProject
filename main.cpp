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
    int start = clock();
    int outcouplingCount = 0;
    vector<Ray> RAYS;
    vector<Ray> testRays;
    testRays.push_back(Ray(Vector2(500.0, 350.0), Vector2(0.0, 1.0), 0, 1));
    // testRays.push_back(Ray(Vector2(500.0, 550.0), Vector2(1.0, 1.0), 0, 1));
    // testRays.push_back(Ray(Vector2(500.0, 550.0), Vector2(-1.0, 1.0), 0, 1));
    // testRays.push_back(Ray(Vector2(500.0, 550.0), Vector2(0.01, -1.0), 0, 1));

    // RAYS.insert(RAYS.end(), testRays.begin(), testRays.end());

    for (int i = 0; i < 1024; i++){ // Generating Random Rays
        RAYS.push_back(Ray(Vector2(500, 350), RndDir(), 0, 1));
    }

    vector<Layer> testLayers; //! MUST DEFINE LINES LEFT TO RIGHT FOR CORRECT NORMAL OF LAYERS
    testLayers.push_back(Layer( { Vector2(100, 200), Vector2(900, 200)}, Reflective)); // metal - Bphen
    testLayers.push_back(Layer( { Vector2(100, 300), Vector2(900, 300)}, Refractive)); // Bphen - Irppy
    testLayers.push_back(Layer( { Vector2(100, 400), Vector2(900, 400)}, Refractive)); // Irppy - TCTA
    testLayers.push_back(Layer( { Vector2(100, 500), Vector2(900, 500)}, Refractive)); // TCTA - ITO
    testLayers.push_back(Layer( { Vector2(100, 600), Vector2(900, 600)}, Refractive)); // ITO - Glass
    testLayers.push_back(Layer( { Vector2(100, 700), Vector2(900, 700)}, Refractive)); // Glass - Air
    testLayers.push_back(Layer( { Vector2( 50, 710), Vector2(950, 710)}, Outcoupled));
    testLayers.push_back(Layer( { Vector2(900, 200), Vector2(900, 700)}, Reflective)); // sides
    testLayers.push_back(Layer( { Vector2(100, 200), Vector2(100, 700)}, Reflective)); // sides
    testLayers.push_back(Layer( { Vector2( 50, 700), Vector2(50, 100), Vector2(950, 100), Vector2(950, 700)}, Reflective)); // sides

    vector<float> refractiveIndexes = {1.710, 1.69, 1.718, 1.8270, 1.5, 1}; // All refractive Indexes, Bphen, irppy, TCTA , ITO, glass, Air
    // vector<float> refractiveIndexes = {1.710, 1.69, 4, 3, 2, 1};

    for (const Ray& ray : RAYS){ // Testing Refraction
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(0,1)).direction.angle() / PI << " ";
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(-1,-1).normalized()).direction.angle() / PI << " ";
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(1,1).normalized()).direction.angle() / PI << "\n";
    } // 90 85.0027 85.0027 /n 49.9973 45 45
    
    size_t i = 0;
    while (i < RAYS.size()){
        Ray& ray = RAYS[i];
        if (ray.getBounces() < 1000){
            HitInfo closestHit;
            closestHit.didHit = false;
            for (const Layer& layer : testLayers){ // Go through Layers Of Device
                HitInfo hit = RAYNAME::collisionDetection(ray, layer.getPoints());
                if (!closestHit.didHit || hit.distance < closestHit.distance){
                    closestHit = hit;
                    closestHit.type = layer.getLayerType();
                }
            }
            // TODO Check if ray hits a side (edge case : )
            if (closestHit.didHit){
                RefractedRay newAngle;
                int newRefLayerIndex = ray.getRefLayerIndex();
                switch (closestHit.type){
                case 0: // Refractive Layer
                    newAngle = refractedAngle(refractiveIndexes[ray.getRefLayerIndex()], 
                                             (ray.getDirection().dot(closestHit.normal) > 0) ? refractiveIndexes[ray.getRefLayerIndex() + 1] : 
                                              refractiveIndexes[ray.getRefLayerIndex() - 1],
                                              ray, closestHit.normal);
                    if (newAngle.type == REFRACTION) {
                        if (ray.getDirection().dot(closestHit.normal) > 0)
                            newRefLayerIndex += 1; // Moving Up (towards Air)
                        else
                            newRefLayerIndex -= 1; // Moving Down
                    }
                    RAYS.push_back(Ray(closestHit.hitPoint, newAngle.direction, ray.getBounces() + 1, newRefLayerIndex));
                    break;

                case 1: // Reflective
                    // Add chance to reflect dice roll
                    if (ray.getDirection().cross(closestHit.normal) == 0) // TODO Refactor into Refraction Code
                        newAngle.direction = -ray.getDirection();
                    else if (ray.getDirection().cross(closestHit.normal) > 0){
                        double angleWithNormal = acos(ray.getDirection().dot(closestHit.normal) / (ray.getDirection().length() * closestHit.normal.length()));
                        newAngle.direction = Vector2(cos(closestHit.normal.angle() + PI + angleWithNormal), sin(closestHit.normal.angle() + PI + angleWithNormal));
                    }
                    else{
                        double angleWithNormal = acos(ray.getDirection().dot(closestHit.normal) / (ray.getDirection().length() * closestHit.normal.length()));
                        newAngle.direction = Vector2(cos(closestHit.normal.angle() + PI - angleWithNormal), sin(closestHit.normal.angle() + PI - angleWithNormal));
                    }
                    RAYS.push_back(Ray(closestHit.hitPoint, newAngle.direction, ray.getBounces() + 1, ray.getRefLayerIndex()));
                    break;
                // case 2: // Transport (move to other side)
                //     break;
                case 3: // Exit
                    // add to outcoupling counter
                    outcouplingCount++;
                    break;
                default:
                    break;
                }
            }
            /*
            !redesign
            Absorption chance roll
            Reflection chance roll
            Refraction -> reflect or refract

            */
        }
        ++i;
    }

    int end = clock();
    std::cout << double(end-start)/CLOCKS_PER_SEC;
    cout << '\n' << ">>" << outcouplingCount;

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
        for (const Ray& ray : RAYS) {
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

    std::cout << "\nend\n";
    string test; // Leave consol open
    std::cin >> test;
    return 0;
}

// ** RANDOM RAYS
// vector<Ray> RAYS; 
    // for (int i = 0; i < 256; i++){ // Generating Random Rays
    //     RAYS.push_back(Ray(Vector2(500, 500), RndDir()));
    // }
    // cout << refractedAngle(1.5, 1, 40*PI/180);

    // srand (time(NULL));