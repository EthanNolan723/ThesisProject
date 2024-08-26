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

#include <SDL2/SDL.h>

using namespace std;

#define SDL_WINDOW_WIDTH 1000
#define SDL_WINDOW_HEIGHT 1500
#define TOTALRAYS 600
#define OLEDWIDTH 800

Vector2 RndDir(){ // Temp
    double randAngle = 2*PI * rand() / (RAND_MAX + 1);
    return Vector2(sin(randAngle), cos(randAngle));
}

int getRandomInt() {
    random_device rd; // Seed for the random number engine
    mt19937 gen(rd()); // Mersenne Twister engine
    uniform_int_distribution<> distr(0, 255); // Define range 0-255

    return distr(gen);
}

bool areDoublesEqual(double a, double b, double epsilon = 1e-8) { // Temp
    return fabs(a - b) < epsilon;
}

int WinMain(int argc, char* argv[]){
    int start = clock();
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

    RAYS.insert(RAYS.end(), testRays.begin(), testRays.end());

    for (int i = 0; i < TOTALRAYS; i++){ //! Generating Random Rays
        RAYS.push_back(Ray(Vector2(500, 185), RndDir(), 0, 0)); // distribution uniform vs normal - time dependent density theory - dft
        // transition dipole moment 

        //anisotropic light emmission oled material dft - perameter to adjust isotopic value, 1 (perfectly straight up), 0 perfectly isotropic
    }

    vector<Layer> OLEDLayers; //! MUST DEFINE LINES LEFT TO RIGHT FOR CORRECT NORMAL OF LAYERS
    vector<Layer> BarrierLayers; //! MUST DEFINE LINES LEFT TO RIGHT FOR CORRECT NORMAL OF LAYERS
    /*
    //! Basic Set Up of Irppy Design (maybe)
    OLEDLayers.push_back(Layer( { Vector2(100, 200), Vector2(900, 200)}, Reflective)); // metal - Bphen
    OLEDLayers.push_back(Layer( { Vector2(100, 300), Vector2(900, 300)}, Refractive)); // Bphen - Irppy
    OLEDLayers.push_back(Layer( { Vector2(100, 400), Vector2(900, 400)}, Refractive)); // Irppy - TCTA
    OLEDLayers.push_back(Layer( { Vector2(100, 500), Vector2(900, 500)}, Refractive)); // TCTA - ITO
    OLEDLayers.push_back(Layer( { Vector2(100, 600), Vector2(900, 600)}, Refractive)); // ITO - Glass
    OLEDLayers.push_back(Layer( { Vector2(100, 700), Vector2(900, 700)}, Refractive)); // Glass - Air
    OLEDLayers.push_back(Layer( { Vector2( 50, 710), Vector2(950, 710)}, Outcoupled));
    BarrierLayers.push_back(Layer( { Vector2(900, 200), Vector2(900, 700)}, RightWall)); // sides
    BarrierLayers.push_back(Layer( { Vector2(100, 200), Vector2(100, 700)}, LeftWall)); // sides
    OLEDLayers.push_back(Layer( { Vector2( 50, 700), Vector2(50, 100), Vector2(950, 100), Vector2(950, 700)}, Reflective)); // testing Outside
    vector<float> refractiveIndexes = {1.710, 1.69, 1.718, 1.8270, 1.5, 1}; // All refractive Indexes, Bphen, irppy, TCTA , ITO, glass, Air
    vector<float> extinctionCoeficients = {0, 1.08e5, 1.08e5, 1.08e5, 1.08e5, 0};
    */
    OLEDLayers.push_back(Layer( { Vector2(100, 100), Vector2(900, 100)}, Reflective)); // Metal - Organic
    OLEDLayers.push_back(Layer( { Vector2(100, 290), Vector2(900, 290)}, Refractive)); // Organic - ITO
    OLEDLayers.push_back(Layer( { Vector2(100, 440), Vector2(900, 440)}, Refractive)); // ITO - Glass
    OLEDLayers.push_back(Layer( { Vector2(100, 1440), Vector2(900, 1440)}, Refractive)); // Glass - Air
    
    BarrierLayers.push_back(Layer( { Vector2(900, 100), Vector2(900, 1440)}, RightWall)); // sides
    BarrierLayers.push_back(Layer( { Vector2(100, 100), Vector2(100, 1440)}, LeftWall)); // sides
    OLEDLayers.push_back(Layer( { Vector2(40, 1440), Vector2(50, 1490), Vector2(950, 1490), Vector2(960, 1440)}, Outcoupled)); // Outcoupled
    vector<float> refractiveIndexes = {1.78, 1.951, 1.5, 1};
    vector<float> extinctionCoeficients = {0, 1.034e6, 0, 0}; // ! Actually Alpha value

    // //! Testing Corner Itersection
    // BarrierLayers.push_back(Layer( { Vector2(100,100), Vector2(100,400)}, Transport));
    // OLEDLayers.push_back(Layer( { Vector2(100,200), Vector2(300,200)}, Refractive));

    
    // vector<float> refractiveIndexes = {1.710, 1.69, 4, 3, 2, 1};
    // vector<float> refractiveIndexes = {1.7, 1};

    for (const Ray& ray : RAYS){ // Testing Refraction
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(0,1)).direction.angle() / PI << " ";
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(-1,-1).normalized()).direction.angle() / PI << " ";
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(1,1).normalized()).direction.angle() / PI << "\n";
    } // 90 85.0027 85.0027 /n 49.9973 45 45
    
    size_t i = 0;
    while (i < RAYS.size()){
        Ray& ray = RAYS[i];
        if (ray.getBounces() < 300){
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
                switch (closestHit.type){
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
                    newAngle.direction = Reflection(ray, closestHit.normal);
                    newRefLayerIndex = ray.getRefLayerIndex();
                    // Add Extinction
                    break;

                case 2: // Exit
                    // add to outcoupling counter
                    generateNewRay = false;
                    outcouplingCount++;
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
                    if ((float) rand()/RAND_MAX > exp(-extinctionCoeficients[ray.getRefLayerIndex()] * closestHit.distance * 1e-9)){ //! rand , cpp boost lib /check complier options first
                        generateNewRay = false; // Delete Ray If Random Rolls above Intensity drop
                        lostRayCount++;
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
        ++i;
    }

    int end = clock();
    std::cout << double(end-start)/CLOCKS_PER_SEC;
    std::cout << '\n' << "          Rays Out >>" << outcouplingCount;
    std::cout << '\n' << "         Rays lost >>" << lostRayCount;
    std::cout << '\n' << "             % out >>" << (float) outcouplingCount/TOTALRAYS;
    std::cout << '\n' << "rays lost (Bounce) >>" << -lostRayCount - outcouplingCount + TOTALRAYS;

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
                    SDL_SetRenderDrawColor(renderer, 255, 0, getRandomInt(),255);
                    ray.draw(renderer, 75);
                }

                //Draw Layers
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                for (const Layer& layer : OLEDLayers){
                    layer.draw(renderer);
                }
                SDL_SetRenderDrawColor(renderer, 20, 60, 255, 255);
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