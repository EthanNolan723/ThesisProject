#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "refraction.h"
#include "ray.h"
#include "Vector2.h"
#include "layerMaterial.h"

#include "OLEDStructures.h"

#include <SDL2/SDL.h>

using namespace std;

#define SDL_WINDOW_WIDTH 1000
#define SDL_WINDOW_HEIGHT 800

#define TOTALRAYS 10000
#define TOTALBOUNCES 300
#define TOTALRUNS 10000
#define OLEDWIDTH 800
#define WAVELENGTH 510 * 1e-9

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> rnd(0.0, 1.0);
uniform_real_distribution<> rndAngle(0.0, 2 * PI);

Vector2 RndDir() { 
    double randAngle = rndAngle(gen);
    return Vector2(cos(randAngle), sin(randAngle));
}

Vector2 RndDirNormal(double spreadAngle = 10.0) {
    normal_distribution<double> normalRndAngleUp(90.0, spreadAngle);
    normal_distribution<double> normalRndAngleDown(-90.0, spreadAngle);

    // Randomly choose between the two distributions
    uniform_int_distribution<int> chooser(0, 1);
    double randAngle = chooser(gen) == 0 ? normalRndAngleUp(gen) : normalRndAngleDown(gen);

    return Vector2(cos(randAngle * PI / 180.0), sin(randAngle * PI / 180.0));
}

bool areDoublesEqual(double a, double b, double err = 1e-8) { // Temp
    return fabs(a - b) < err;
}

int WinMain(int argc, char* argv[]){
    int start = clock();

    vector<int> outCoupledPercentage;
    initializeLayers();

    for (int run = 0; run < TOTALRUNS; run++){

    }
    int outcouplingCount = 0;
    int lostRayCount = 0;
    vector<Ray> RAYS;
    vector<Ray> testRays;
    // testRays.push_back(Ray(Vector2(200.0, 100.0), Vector2(-1.0, 1.0), 0, 0));
    // testRays.push_back(Ray(Vector2(200.0, 100.0), Vector2(-2.0, 1.0), 0, 0));
    // testRays.push_back(Ray(Vector2(500.0, 550.0), Vector2(1.0, 1.0), 0, 1));
    // testRays.push_back(Ray(Vector2(500.0, 550.0), Vector2(-1.0, 1.0), 0, 1));
    // testRays.push_back(Ray(Vector2(500.0, 550.0), Vector2(0.01, -1.0), 0, 1));
    // testRays.push_back(Ray(Vector2(500, 350), Vector2(1.0, 0.1), 0, 1));
    // RAYS.insert(RAYS.end(), testRays.begin(), testRays.end());

    for (int i = 0; i < TOTALRAYS; i++){ //! Generating Random Rays
        RAYS.push_back(Ray(Vector2(500, 185), RndDir(), 0, 2)); // distribution uniform vs normal - time dependent density theory - dft
        // ?transition dipole moment 
        // ?anisotropic light emmission oled material dft - perameter to adjust isotopic value, 1 (perfectly straight up), 0 perfectly isotropic
    }

    

    // //! Testing Corner Itersection
    // BarrierLayers.push_back(Layer( { Vector2(100,100), Vector2(100,400)}, Transport));
    // OLEDLayers.push_back(Layer( { Vector2(100,200), Vector2(300,200)}, Refractive));
    // vector<float> refractiveIndexes = {1.710, 1.69, 4, 3, 2, 1};
    // vector<float> refractiveIndexes = {1.7, 1};

    size_t i = 0;
    // *Collisions: Run though all rays and check for collision
    // *Add new ray to end of list, adjusted for refraction/reflection ect
    while (i < RAYS.size()){
        Ray& ray = RAYS[i];
        if (ray.getBounces() < TOTALBOUNCES){
            HitInfo closestHit;
            closestHit.didHit = false;
            for (const Layer& layer : OLEDLayers){ // Go through Layers Of Device
                HitInfo hit = RAYNAMESPACE::collisionDetection(ray, layer);
                if (!closestHit.didHit || hit.distance < closestHit.distance){
                    closestHit = hit;
                }
            }
            for (const Layer& layer : BarrierLayers){ // ? Assumes you can't hit a refractive layer AFTER a transport layer
                HitInfo hit = RAYNAMESPACE::collisionDetection(ray, layer);
                if (!closestHit.didHit || hit.distance < (closestHit.distance - 1e-8)){ // If it collides before the layer
                    closestHit = hit;
                    closestHit.hitPoint += (hit.type == LeftWall) ? Vector2(OLEDWIDTH, 0) : Vector2(-OLEDWIDTH, 0);
                }
                else if (hit.didHit && areDoublesEqual(hit.distance, closestHit.distance)){ // If they collide at the same point
                    closestHit.hitPoint += (hit.type == LeftWall) ? Vector2(OLEDWIDTH, 0) : Vector2(-OLEDWIDTH, 0); // Check if it is left or right
                }
            }
            if (closestHit.didHit){
                RefractedRay newAngle;
                bool generateNewRay = true;
                int newRefLayerIndex = ray.getRefLayerIndex();
                ray.setLength(closestHit.distance);
                switch (closestHit.type){  // Collision Type
                case 0: // Refractive Layer
                    newAngle = refractedAngle(refractiveIndexes[ray.getRefLayerIndex()],
                                             (ray.getDirection().dot(closestHit.normal) > 0) ? refractiveIndexes[ray.getRefLayerIndex() + 1] : 
                                              refractiveIndexes[ray.getRefLayerIndex() - 1],
                                              ray, closestHit.normal);
                    if (newAngle.type == REFRACTION) { // If light reflected update
                        if (ray.getDirection().dot(closestHit.normal) > 0)
                            newRefLayerIndex += 1; // Moving Up (towards Air)
                        else
                            newRefLayerIndex -= 1; // Moving Down
                    }
                    break;

                case 1: // Reflective
                    if ((float) rand()/RAND_MAX < 0.30){
                        newAngle.direction = Reflection(ray, closestHit.normal);
                        newRefLayerIndex = ray.getRefLayerIndex();
                    }
                    else{
                        generateNewRay = false;
                        ray.setRayColour(Colour{0,0,255,255});
                    }
                    break;

                case 2: // Exit
                    // add to outcoupling counter
                    generateNewRay = false;
                    outcouplingCount++;
                    ray.setRayColour(Colour{0,255,0,255});
                    break;
                    
                case 3: // Side Walls
                case 4:
                    newAngle.direction = ray.getDirection();
                    newRefLayerIndex = ray.getRefLayerIndex();
                    break;

                default:
                    break;
                }

                if (extinctionCoeficients[ray.getRefLayerIndex()] != 0 && generateNewRay){ //Assume intensity always = 100%
                //! UPDATE RAND NUMBER GEN - ADD IN DISTANCE REPRESENTATION ( draw line for 50% intesity drop)
                    if (rnd(gen) > exp(-extinctionCoeficients[ray.getRefLayerIndex()] * closestHit.distance * 1e-9)){ //! rand , cpp boost lib /check complier options first
                        generateNewRay = false; // StopRay if Random Rolls above Intensity drop
                        lostRayCount++;
                        ray.setRayColour(Colour{0,0,255,255});
                        ray.setLength(ray.getLength() * exp(-extinctionCoeficients[ray.getRefLayerIndex()] * closestHit.distance * 1e-9));
                    }                        
                }
                
                if (generateNewRay){
                    RAYS.push_back(Ray(closestHit.hitPoint, newAngle.direction, ray.getBounces() + 1, newRefLayerIndex));
                }
            }
            /*
            !redesign
            Absorption chance roll
            Reflection chance roll
            Refraction -> reflect or refract
            */
        }
        else
            ray.setRayColour(Colour{255,0,255,255});
        ++i;
    }

    outCoupledPercentage.push_back((float) outcouplingCount/TOTALRAYS);



    int end = clock();

    
    // Display Outputs of single run
    // std::cout << '\n' << "             Time >> " << double(end-start)/CLOCKS_PER_SEC;
    // std::cout << '\n' << "         Rays Out >> " << outcouplingCount;
    // std::cout << '\n' << "(Absorbtion) Lost >> " << lostRayCount;
    // std::cout << '\n' << "    (Bounce) lost >> " << -lostRayCount - outcouplingCount + TOTALRAYS;
    // std::cout << '\n' << "            % out >> " << (float) outcouplingCount/TOTALRAYS;

    //Create Empty Window
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",                    
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOW_WIDTH,
        SDL_WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
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
                // Clear screen
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // Draw Rays
                for (const Ray& ray : RAYS) { // Add distance 
                    SDL_SetRenderDrawColor(renderer, ray.getRayColour().r, ray.getRayColour().g, ray.getRayColour().b, ray.getRayColour().a);
                    ray.draw(renderer, (int) ray.getLength());
                }

                //Draw Layers
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                for (const Layer& layer : OLEDLayers){
                    layer.draw(renderer);
                }
                SDL_SetRenderDrawColor(renderer, 150, 255, 0, 255);
                for (const Layer& layer : BarrierLayers){
                    layer.draw(renderer);
                }
                
                SDL_RenderPresent(renderer);
        }
        
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