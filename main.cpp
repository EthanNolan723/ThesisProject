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

// https://pubs.acs.org/doi/full/10.1021/acs.nanolett.7b03528

using namespace std;

#define SDL_WINDOW_WIDTH 1000
#define SDL_WINDOW_HEIGHT 800

#define TOTALRAYS 1000000000
#define TOTALBOUNCES 1000
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
    
    //Flat
    OLEDLayers.push_back(Layer( { Vector2(100, 10), Vector2(900, 10)}, Refractive)); // glass - metal
    OLEDLayers.push_back(Layer( { Vector2(100, 100), Vector2(900, 100)}, Refractive)); // Metal - Organic
    OLEDLayers.push_back(Layer( { Vector2(100, 290), Vector2(900, 290)}, Refractive)); // Organic - ITO
    OLEDLayers.push_back(Layer( { Vector2(100, 440), Vector2(900, 440)}, Refractive)); // ITO - Glass
    OLEDLayers.push_back(Layer( { Vector2(100, 1440), Vector2(900, 1440)}, Refractive)); // Glass - Air
    BarrierLayers.push_back(Layer( { Vector2(900, 10), Vector2(900, 1440)}, RightWall)); // sides
    BarrierLayers.push_back(Layer( { Vector2(100, 10), Vector2(100, 1440)}, LeftWall)); // sides
    OLEDLayers.push_back(Layer( { Vector2(40, 1440), Vector2(50, 1490), Vector2(950, 1490), Vector2(960, 1440)}, Outcoupled)); // Outcoupled
    vector<float> refractiveIndexes = {1.5, 0.85, 1.78, 1.951, 1.5, 1};
    vector<float> extinctionCoeficients = {0, 147.8e6, 0, 1.058e6, 0, 0}; // ! Alpha Values - not n2 - 4 pi n2 / wavelegnth
    
    
    /*// Short Flat
    OLEDLayers.push_back(Layer( { Vector2(100, 10), Vector2(900, 10)}, Refractive)); // glass - metal
    OLEDLayers.push_back(Layer( { Vector2(100, 100), Vector2(900, 100)}, Refractive)); // Metal - Organic
    OLEDLayers.push_back(Layer( { Vector2(100, 300), Vector2(900, 300)}, Refractive)); // Organic - ITO
    OLEDLayers.push_back(Layer( { Vector2(100, 400), Vector2(900, 400)}, Refractive)); // ITO - Glass
    OLEDLayers.push_back(Layer( { Vector2(100, 500), Vector2(900, 500)}, Refractive)); // Glass - Air
    BarrierLayers.push_back(Layer( { Vector2(900, 10), Vector2(900, 500)}, RightWall)); // sides
    BarrierLayers.push_back(Layer( { Vector2(100, 10), Vector2(100, 500)}, LeftWall)); // sides
    OLEDLayers.push_back(Layer( { Vector2(40, 500), Vector2(50, 510), Vector2(950, 510), Vector2(960, 500)}, Outcoupled)); // Outcoupled
    vector<float> refractiveIndexes = {1.5, 0.85, 1.78, 1.951, 1.5, 1};
    vector<float> extinctionCoeficients = {0, 147.8e6, 0, 1.058e6, 0, 0}; // ! Alpha Values - not n2 - 4 pi n2 / wavelegnth
    */

    /*//Sin wave Organic / ITO, P-P 10:
    OLEDLayers.push_back(Layer( { Vector2(100, 100), Vector2(900, 100)}, Refractive)); // Metal - Organic
    OLEDLayers.push_back(Layer( {
    Vector2(100, 290), Vector2(105, 290.975451610081), Vector2(110, 291.913417161825), Vector2(115, 292.777851165098), Vector2(120, 293.535533905933),
    Vector2(125, 294.157348061513), Vector2(130, 294.619397662556), Vector2(135, 294.903926402016), Vector2(140, 295),
    Vector2(145, 294.903926402016), Vector2(150, 294.619397662556), Vector2(155, 294.157348061513), Vector2(160, 293.535533905933),
    Vector2(165, 292.777851165098), Vector2(170, 291.913417161825), Vector2(175, 290.975451610081), Vector2(180, 290),
    Vector2(185, 289.024548389919), Vector2(190, 288.086582838175), Vector2(195, 287.222148834902), Vector2(200, 286.464466094067),
    Vector2(205, 285.842651938487), Vector2(210, 285.380602337444), Vector2(215, 285.096073597984), Vector2(220, 285),
    Vector2(225, 285.096073597984), Vector2(230, 285.380602337444), Vector2(235, 285.842651938487), Vector2(240, 286.464466094067),
    Vector2(245, 287.222148834902), Vector2(250, 288.086582838175), Vector2(255, 289.024548389919), Vector2(260, 290),
    Vector2(265, 290.975451610081), Vector2(270, 291.913417161825), Vector2(275, 292.777851165098), Vector2(280, 293.535533905933),
    Vector2(285, 294.157348061513), Vector2(290, 294.619397662556), Vector2(295, 294.903926402016), Vector2(300, 295),
    Vector2(305, 294.903926402016), Vector2(310, 294.619397662556), Vector2(315, 294.157348061513), Vector2(320, 293.535533905933),
    Vector2(325, 292.777851165098), Vector2(330, 291.913417161825), Vector2(335, 290.975451610081), Vector2(340, 290),
    Vector2(345, 289.024548389919), Vector2(350, 288.086582838175), Vector2(355, 287.222148834902), Vector2(360, 286.464466094067),
    Vector2(365, 285.842651938487), Vector2(370, 285.380602337444), Vector2(375, 285.096073597984), Vector2(380, 285),
    Vector2(385, 285.096073597984), Vector2(390, 285.380602337444), Vector2(395, 285.842651938487), Vector2(400, 286.464466094067),
    Vector2(405, 287.222148834902), Vector2(410, 288.086582838175), Vector2(415, 289.024548389919), Vector2(420, 290),
    Vector2(425, 290.975451610081), Vector2(430, 291.913417161825), Vector2(435, 292.777851165098), Vector2(440, 293.535533905933),
    Vector2(445, 294.157348061513), Vector2(450, 294.619397662556), Vector2(455, 294.903926402016), Vector2(460, 295),
    Vector2(465, 294.903926402016), Vector2(470, 294.619397662556), Vector2(475, 294.157348061513), Vector2(480, 293.535533905933),
    Vector2(485, 292.777851165098), Vector2(490, 291.913417161825), Vector2(495, 290.975451610081), Vector2(500, 290),
    Vector2(505, 289.024548389919), Vector2(510, 288.086582838175), Vector2(515, 287.222148834902), Vector2(520, 286.464466094067),
    Vector2(525, 285.842651938487), Vector2(530, 285.380602337444), Vector2(535, 285.096073597984), Vector2(540, 285),
    Vector2(545, 285.096073597984), Vector2(550, 285.380602337444), Vector2(555, 285.842651938487), Vector2(560, 286.464466094067),
    Vector2(565, 287.222148834902), Vector2(570, 288.086582838175), Vector2(575, 289.024548389919), Vector2(580, 290),
    Vector2(585, 290.975451610081), Vector2(590, 291.913417161825), Vector2(595, 292.777851165098), Vector2(600, 293.535533905933),
    Vector2(605, 294.157348061513), Vector2(610, 294.619397662556), Vector2(615, 294.903926402016), Vector2(620, 295),
    Vector2(625, 294.903926402016), Vector2(630, 294.619397662556), Vector2(635, 294.157348061513), Vector2(640, 293.535533905933),
    Vector2(645, 292.777851165098), Vector2(650, 291.913417161825), Vector2(655, 290.975451610081), Vector2(660, 290),
    Vector2(665, 289.024548389919), Vector2(670, 288.086582838175), Vector2(675, 287.222148834902), Vector2(680, 286.464466094067),
    Vector2(685, 285.842651938487), Vector2(690, 285.380602337444), Vector2(695, 285.096073597984), Vector2(700, 285),
    Vector2(705, 285.096073597984), Vector2(710, 285.380602337444), Vector2(715, 285.842651938487), Vector2(720, 286.464466094067),
    Vector2(725, 287.222148834902), Vector2(730, 288.086582838175), Vector2(735, 289.024548389919), Vector2(740, 290),
    Vector2(745, 290.975451610081), Vector2(750, 291.913417161825), Vector2(755, 292.777851165098), Vector2(760, 293.535533905933),
    Vector2(765, 294.157348061513), Vector2(770, 294.619397662556), Vector2(775, 294.903926402016), Vector2(780, 295),
    Vector2(785, 294.903926402016), Vector2(790, 294.619397662556), Vector2(795, 294.157348061513), Vector2(800, 293.535533905933),
    Vector2(805, 292.777851165098), Vector2(810, 291.913417161825), Vector2(815, 290.975451610081), Vector2(820, 290),
    Vector2(825, 289.024548389919), Vector2(830, 288.086582838175), Vector2(835, 287.222148834902), Vector2(840, 286.464466094067),
    Vector2(845, 285.842651938487), Vector2(850, 285.380602337444), Vector2(855, 285.096073597984), Vector2(860, 285),
    Vector2(865, 285.096073597984), Vector2(870, 285.380602337444), Vector2(875, 285.842651938487), Vector2(880, 286.464466094067),
    Vector2(885, 287.222148834902), Vector2(890, 288.086582838175), Vector2(895, 289.024548389919), Vector2(900, 290)
    }, Refractive)); // Organic - ITO
    OLEDLayers.push_back(Layer( { Vector2(100, 440), Vector2(900, 440)}, Refractive)); // ITO - Glass
    OLEDLayers.push_back(Layer( { Vector2(100, 1440), Vector2(900, 1440)}, Refractive)); // Glass - Air
    BarrierLayers.push_back(Layer( { Vector2(900, 10), Vector2(900, 1440)}, RightWall)); // sides
    BarrierLayers.push_back(Layer( { Vector2(100, 10), Vector2(100, 1440)}, LeftWall)); // sides
    OLEDLayers.push_back(Layer( { Vector2(40, 1440), Vector2(50, 1490), Vector2(950, 1490), Vector2(960, 1440)}, Outcoupled)); // Outcoupled
    vector<float> refractiveIndexes = {1.5, 0.85, 1.78, 1.951, 1.5, 1};
    vector<float> extinctionCoeficients = {0, 147.8e6, 0, 1.058e6, 0, 0}; // ! Alpha Values - not n2 - 4 pi n2 / wavelegnth
    */

    /*//Sin wave Organic / ITO, P-P 20:
    OLEDLayers.push_back(Layer( { Vector2(100, 100), Vector2(900, 100)}, Refractive)); // Metal - Organic
    OLEDLayers.push_back(Layer( { Vector2(100, 290), Vector2(105, 291.950903220161), Vector2(110, 293.826834323651),
    Vector2(115, 295.555702330196), Vector2(120, 297.071067811866),
    Vector2(125, 298.314696123025), Vector2(130, 299.238795325113),
    Vector2(135, 299.807852804032), Vector2(140, 300),
    Vector2(145, 299.807852804032), Vector2(150, 299.238795325113),
    Vector2(155, 298.314696123025), Vector2(160, 297.071067811866),
    Vector2(165, 295.555702330196), Vector2(170, 293.826834323651),
    Vector2(175, 291.950903220161), Vector2(180, 290),
    Vector2(185, 288.049096779839), Vector2(190, 286.173165676349),
    Vector2(195, 284.444297669804), Vector2(200, 282.928932188135),
    Vector2(205, 281.685303876975), Vector2(210, 280.761204674887),
    Vector2(215, 280.192147195968), Vector2(220, 280),
    Vector2(225, 280.192147195968), Vector2(230, 280.761204674887),
    Vector2(235, 281.685303876975), Vector2(240, 282.928932188135),
    Vector2(245, 284.444297669804), Vector2(250, 286.173165676349),
    Vector2(255, 288.049096779839), Vector2(260, 290),
    Vector2(265, 291.950903220161), Vector2(270, 293.826834323651),
    Vector2(275, 295.555702330196), Vector2(280, 297.071067811866),
    Vector2(285, 298.314696123025), Vector2(290, 299.238795325113),
    Vector2(295, 299.807852804032), Vector2(300, 300),
    Vector2(305, 299.807852804032), Vector2(310, 299.238795325113),
    Vector2(315, 298.314696123025), Vector2(320, 297.071067811866),
    Vector2(325, 295.555702330196), Vector2(330, 293.826834323651),
    Vector2(335, 291.950903220161), Vector2(340, 290),
    Vector2(345, 288.049096779839), Vector2(350, 286.173165676349),
    Vector2(355, 284.444297669804), Vector2(360, 282.928932188135),
    Vector2(365, 281.685303876975), Vector2(370, 280.761204674887),
    Vector2(375, 280.192147195968), Vector2(380, 280),
    Vector2(385, 280.192147195968), Vector2(390, 280.761204674887),
    Vector2(395, 281.685303876975), Vector2(400, 282.928932188135),
    Vector2(405, 284.444297669804), Vector2(410, 286.173165676349),
    Vector2(415, 288.049096779839), Vector2(420, 290),
    Vector2(425, 291.950903220161), Vector2(430, 293.826834323651),
    Vector2(435, 295.555702330196), Vector2(440, 297.071067811866),
    Vector2(445, 298.314696123025), Vector2(450, 299.238795325113),
    Vector2(455, 299.807852804032), Vector2(460, 300),
    Vector2(465, 299.807852804032), Vector2(470, 299.238795325113),
    Vector2(475, 298.314696123025), Vector2(480, 297.071067811866),
    Vector2(485, 295.555702330196), Vector2(490, 293.826834323651),
    Vector2(495, 291.950903220161), Vector2(500, 290),
    Vector2(505, 288.049096779839), Vector2(510, 286.173165676349),
    Vector2(515, 284.444297669804), Vector2(520, 282.928932188135),
    Vector2(525, 281.685303876975), Vector2(530, 280.761204674887),
    Vector2(535, 280.192147195968), Vector2(540, 280),
    Vector2(545, 280.192147195968), Vector2(550, 280.761204674887),
    Vector2(555, 281.685303876975), Vector2(560, 282.928932188135),
    Vector2(565, 284.444297669804), Vector2(570, 286.173165676349),
    Vector2(575, 288.049096779839), Vector2(580, 290),
    Vector2(585, 291.950903220161), Vector2(590, 293.826834323651),
    Vector2(595, 295.555702330196), Vector2(600, 297.071067811866),
    Vector2(605, 298.314696123025), Vector2(610, 299.238795325113),
    Vector2(615, 299.807852804032), Vector2(620, 300),
    Vector2(625, 299.807852804032), Vector2(630, 299.238795325113),
    Vector2(635, 298.314696123025), Vector2(640, 297.071067811866),
    Vector2(645, 295.555702330196), Vector2(650, 293.826834323651),
    Vector2(655, 291.950903220161), Vector2(660, 290),
    Vector2(665, 288.049096779839), Vector2(670, 286.173165676349),
    Vector2(675, 284.444297669804), Vector2(680, 282.928932188135),
    Vector2(685, 281.685303876975), Vector2(690, 280.761204674887),
    Vector2(695, 280.192147195968), Vector2(700, 280),
    Vector2(705, 280.192147195968), Vector2(710, 280.761204674887),
    Vector2(715, 281.685303876975), Vector2(720, 282.928932188135),
    Vector2(725, 284.444297669804), Vector2(730, 286.173165676349),
    Vector2(735, 288.049096779839), Vector2(740, 290),
    Vector2(745, 291.950903220161), Vector2(750, 293.826834323651),
    Vector2(755, 295.555702330196), Vector2(760, 297.071067811866),
    Vector2(765, 298.314696123025), Vector2(770, 299.238795325113),
    Vector2(775, 299.807852804032), Vector2(780, 300),
    Vector2(785, 299.807852804032), Vector2(790, 299.238795325113),
    Vector2(795, 298.314696123025), Vector2(800, 297.071067811866),
    Vector2(805, 295.555702330196), Vector2(810, 293.826834323651),
    Vector2(815, 291.950903220161), Vector2(820, 290),
    Vector2(825, 288.049096779839), Vector2(830, 286.173165676349),
    Vector2(835, 284.444297669804), Vector2(840, 282.928932188135),
    Vector2(845, 281.685303876975), Vector2(850, 280.761204674887),
    Vector2(855, 280.192147195968), Vector2(860, 280),
    Vector2(865, 280.192147195968), Vector2(870, 280.761204674887),
    Vector2(875, 281.685303876975), Vector2(880, 282.928932188135),
    Vector2(885, 284.444297669804), Vector2(890, 286.173165676349),
    Vector2(895, 288.049096779839), Vector2(900, 290) }, Refractive)); // Organic - ITO
    OLEDLayers.push_back(Layer( { Vector2(100, 440), Vector2(900, 440)}, Refractive)); // ITO - Glass
    OLEDLayers.push_back(Layer( { Vector2(100, 1440), Vector2(900, 1440)}, Refractive)); // Glass - Air
    BarrierLayers.push_back(Layer( { Vector2(900, 10), Vector2(900, 1440)}, RightWall)); // sides
    BarrierLayers.push_back(Layer( { Vector2(100, 10), Vector2(100, 1440)}, LeftWall)); // sides
    OLEDLayers.push_back(Layer( { Vector2(40, 1440), Vector2(50, 1490), Vector2(950, 1490), Vector2(960, 1440)}, Outcoupled)); // Outcoupled
    vector<float> refractiveIndexes = {1.5, 0.85, 1.78, 1.951, 1.5, 1};
    vector<float> extinctionCoeficients = {0, 147.8e6, 0, 1.058e6, 0, 0}; // ! Alpha Values - not n2 - 4 pi n2 / wavelegnth
    */

    /*//Sin wave Organic / ITO, P-P 20:
    OLEDLayers.push_back(Layer( { Vector2(100, 100), Vector2(900, 100) }, Refractive)); // Metal - Organic
    OLEDLayers.push_back(Layer( { Vector2(100, 290), Vector2(105, 292.926354830242), Vector2(110, 295.740251485476), 
    Vector2(115, 298.333553495294), Vector2(120, 300.606601717798), Vector2(125, 302.472044184538), 
    Vector2(130, 303.858192987669), Vector2(135, 304.711779206048), Vector2(140, 305), 
    Vector2(145, 304.711779206048), Vector2(150, 303.858192987669), Vector2(155, 302.472044184538), 
    Vector2(160, 300.606601717798), Vector2(165, 298.333553495294), Vector2(170, 295.740251485476), 
    Vector2(175, 292.926354830242), Vector2(180, 290), Vector2(185, 287.073645169758), 
    Vector2(190, 284.259748514524), Vector2(195, 281.666446504706), Vector2(200, 279.393398282202), 
    Vector2(205, 277.527955815462), Vector2(210, 276.141807012331), Vector2(215, 275.288220793952), 
    Vector2(220, 275), Vector2(225, 275.288220793952), Vector2(230, 276.141807012331), 
    Vector2(235, 277.527955815462), Vector2(240, 279.393398282202), Vector2(245, 281.666446504706), 
    Vector2(250, 284.259748514524), Vector2(255, 287.073645169758), Vector2(260, 290), 
    Vector2(265, 292.926354830242), Vector2(270, 295.740251485476), Vector2(275, 298.333553495294), 
    Vector2(280, 300.606601717798), Vector2(285, 302.472044184538), Vector2(290, 303.858192987669), 
    Vector2(295, 304.711779206048), Vector2(300, 305), Vector2(305, 304.711779206048), 
    Vector2(310, 303.858192987669), Vector2(315, 302.472044184538), Vector2(320, 300.606601717798), 
    Vector2(325, 298.333553495294), Vector2(330, 295.740251485476), Vector2(335, 292.926354830242), 
    Vector2(340, 290), Vector2(345, 287.073645169758), Vector2(350, 284.259748514524), 
    Vector2(355, 281.666446504706), Vector2(360, 279.393398282202), Vector2(365, 277.527955815462), 
    Vector2(370, 276.141807012331), Vector2(375, 275.288220793952), Vector2(380, 275), 
    Vector2(385, 275.288220793952), Vector2(390, 276.141807012331), Vector2(395, 277.527955815462), 
    Vector2(400, 279.393398282202), Vector2(405, 281.666446504706), Vector2(410, 284.259748514524), 
    Vector2(415, 287.073645169758), Vector2(420, 290), Vector2(425, 292.926354830242), 
    Vector2(430, 295.740251485476), Vector2(435, 298.333553495294), Vector2(440, 300.606601717798), 
    Vector2(445, 302.472044184538), Vector2(450, 303.858192987669), Vector2(455, 304.711779206048), 
    Vector2(460, 305), Vector2(465, 304.711779206048), Vector2(470, 303.858192987669), 
    Vector2(475, 302.472044184538), Vector2(480, 300.606601717798), Vector2(485, 298.333553495294), 
    Vector2(490, 295.740251485476), Vector2(495, 292.926354830242), Vector2(500, 290), 
    Vector2(505, 287.073645169758), Vector2(510, 284.259748514524), Vector2(515, 281.666446504706), 
    Vector2(520, 279.393398282202), Vector2(525, 277.527955815462), Vector2(530, 276.141807012331), 
    Vector2(535, 275.288220793952), Vector2(540, 275), Vector2(545, 275.288220793952), 
    Vector2(550, 276.141807012331), Vector2(555, 277.527955815462), Vector2(560, 279.393398282202), 
    Vector2(565, 281.666446504706), Vector2(570, 284.259748514524), Vector2(575, 287.073645169758), 
    Vector2(580, 290), Vector2(585, 292.926354830242), Vector2(590, 295.740251485476), 
    Vector2(595, 298.333553495294), Vector2(600, 300.606601717798), Vector2(605, 302.472044184538), 
    Vector2(610, 303.858192987669), Vector2(615, 304.711779206048), Vector2(620, 305), 
    Vector2(625, 304.711779206048), Vector2(630, 303.858192987669), Vector2(635, 302.472044184538), 
    Vector2(640, 300.606601717798), Vector2(645, 298.333553495294), Vector2(650, 295.740251485476), 
    Vector2(655, 292.926354830242), Vector2(660, 290), Vector2(665, 287.073645169758), 
    Vector2(670, 284.259748514524), Vector2(675, 281.666446504706), Vector2(680, 279.393398282202), 
    Vector2(685, 277.527955815462), Vector2(690, 276.141807012331), Vector2(695, 275.288220793952), 
    Vector2(700, 275), Vector2(705, 275.288220793952), Vector2(710, 276.141807012331), 
    Vector2(715, 277.527955815462), Vector2(720, 279.393398282202), Vector2(725, 281.666446504706), 
    Vector2(730, 284.259748514524), Vector2(735, 287.073645169758), Vector2(740, 290), 
    Vector2(745, 292.926354830242), Vector2(750, 295.740251485476), Vector2(755, 298.333553495294), 
    Vector2(760, 300.606601717798), Vector2(765, 302.472044184538), Vector2(770, 303.858192987669), 
    Vector2(775, 304.711779206048), Vector2(780, 305), Vector2(785, 304.711779206048), 
    Vector2(790, 303.858192987669), Vector2(795, 302.472044184538), Vector2(800, 300.606601717798), 
    Vector2(805, 298.333553495294), Vector2(810, 295.740251485476), Vector2(815, 292.926354830242), 
    Vector2(820, 290), Vector2(825, 287.073645169758), Vector2(830, 284.259748514524), 
    Vector2(835, 281.666446504706), Vector2(840, 279.393398282202), Vector2(845, 277.527955815462), 
    Vector2(850, 276.141807012331), Vector2(855, 275.288220793952), Vector2(860, 275), 
    Vector2(865, 275.288220793952), Vector2(870, 276.141807012331), Vector2(875, 277.527955815462), 
    Vector2(880, 279.393398282202), Vector2(885, 281.666446504706), Vector2(890, 284.259748514524), 
    Vector2(895, 287.073645169758), Vector2(900, 290) }, Refractive)); // Organic - ITO
    OLEDLayers.push_back(Layer( { Vector2(100, 440), Vector2(900, 440) }, Refractive)); // ITO - Glass
    OLEDLayers.push_back(Layer( { Vector2(100, 1440), Vector2(900, 1440) }, Refractive)); // Glass - Air
    BarrierLayers.push_back(Layer( { Vector2(900, 10), Vector2(900, 1440)}, RightWall)); // sides
    BarrierLayers.push_back(Layer( { Vector2(100, 10), Vector2(100, 1440)}, LeftWall)); // sides
    OLEDLayers.push_back(Layer( { Vector2(40, 1440), Vector2(50, 1490), Vector2(950, 1490), Vector2(960, 1440)}, Outcoupled)); // Outcoupled
    vector<float> refractiveIndexes = {1.5, 0.85, 1.78, 1.951, 1.5, 1};
    vector<float> extinctionCoeficients = {0, 147.8e6, 0, 1.058e6, 0, 0}; // ! Alpha Values - not n2 - 4 pi n2 / wavelegnth
    */


    

    // //! Testing Corner Itersection
    // BarrierLayers.push_back(Layer( { Vector2(100,100), Vector2(100,400)}, Transport));
    // OLEDLayers.push_back(Layer( { Vector2(100,200), Vector2(300,200)}, Refractive));
    // vector<float> refractiveIndexes = {1.710, 1.69, 4, 3, 2, 1};
    // vector<float> refractiveIndexes = {1.7, 1};

//!testing
/*
    for (const Ray& ray : RAYS){ // Testing Refraction
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(0,1)).direction.angle() / PI << " ";
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(-1,-1).normalized()).direction.angle() / PI << " ";
        std::cout << 180 * refractedAngle(1, 1.1, ray, Vector2(1,1).normalized()).direction.angle() / PI << "\n";
    } // 90 85.0027 85.0027 /n 49.9973 45 45
*/
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

    int end = clock();
    std::cout << '\n' << "             Time >> " << double(end-start)/CLOCKS_PER_SEC;
    std::cout << '\n' << "         Rays Out >> " << outcouplingCount;
    std::cout << '\n' << "(Absorbtion) Lost >> " << lostRayCount;
    std::cout << '\n' << "    (Bounce) lost >> " << -lostRayCount - outcouplingCount + TOTALRAYS;
    std::cout << '\n' << "            % out >> " << (float) outcouplingCount/TOTALRAYS;

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
                // for (const Ray& ray : RAYS) { // Add distance 
                //     SDL_SetRenderDrawColor(renderer, ray.getRayColour().r, ray.getRayColour().g, ray.getRayColour().b, ray.getRayColour().a);
                //     ray.draw(renderer, (int) ray.getLength());
                // }

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