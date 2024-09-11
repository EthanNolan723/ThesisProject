#ifndef OLEDSTRUCTURES_H
#define OLEDSTRUCTURES_H

#include <vector>
#include "Vector2.h"
#include "layerMaterial.h"

using namespace std;

extern vector<Layer> OLEDLayers;
extern vector<Layer> BarrierLayers;
extern vector<float> refractiveIndexes;
extern vector<float> extinctionCoeficients;

void initializeLayers(int layerPP); // Function to initialize the layers

#endif // OLEDSTRUCTURES_H