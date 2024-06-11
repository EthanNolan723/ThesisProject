#ifndef LAYERMATERIAL_H
#define LAYERMATERIAL_H

#include <cmath>
#include <functional>

using namespace std;

struct layer{
    private:
    float refractiveIndex;
    function<double(double)> func;

    public:
    // Constructors
    layer() : refractiveIndex(1), func([](double) { return 0; }) {};
    layer(float refrIndex, function<double(double)> equation)
        : refractiveIndex(refrIndex), func(equation) {}
    // 
    float getRefInd() const { return refractiveIndex; }

};

#endif