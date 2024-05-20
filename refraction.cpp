#include "refraction.h"

float refractedAngle(float n1, float n2, float angle1){
    if ((n1 > n2) && (angle1 > asin(n2/n1))){
        return (-1*angle1);    
    }
    return asin(n1/n2 * sin(angle1));
}
