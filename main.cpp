#include <iostream>
#include <vector>
#include <string>
#include "test.h"

using namespace std;

int main()
{
    
    float msg = refractedAngle(1.5, 1, 70*PI/180) * 180 / (PI);
    cout << msg << " ";
    cout << PI;
    return 0;
}