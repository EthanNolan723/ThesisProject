#include <iostream>
#include <vector>
#include <string>
#include "refraction.h"
#include "ray.h"

using namespace std;

int main(){
    float msg = refractedAngle(1.1, 1, 40*PI/180) * 180 / (PI);
    cout << msg << " ";

    Ray test;

    return 0;
}