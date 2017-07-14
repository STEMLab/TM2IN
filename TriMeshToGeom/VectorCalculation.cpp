#include "VectorCalculation.h"

#define PI 3.14159265

double VectorCalculation::getAngle(Vector_3& nv1, Vector_3& nv2){
    double cos = (nv1 * nv2) / ( sqrt(nv1.squared_length()) * sqrt(nv2.squared_length()) );
    return acos(cos) * 180.0/PI;
}

// 0 : up, down, left , right, front, back
int VectorCalculation::findNormalType(Vector_3& nv)
{
    int type = 0;
    double diff = 90.0;
    for (int i = 0 ; i < 6 ; i++){
        double temp_diff = VectorCalculation::getAngle(VectorCalculation::normal_list[i], nv);
        if (temp_diff < diff){
            diff = temp_diff;
            type = i;
        }
    }
    return type;
}

Vector_3 VectorCalculation::normal_list[6] = {
    Vector_3(0,0,1),
    Vector_3(0,0,-1),
    Vector_3(1,0,0),
    Vector_3(-1,0,0),
    Vector_3(0,1,0),
    Vector_3(0,-1,0)
};
