//
// Created by dongmin on 18. 1. 18.
//

#include "VertexComputation.h"
#include "predefine.h"

int VertexComputation::compare(Vertex* a, Vertex* b){
    ll multiple = pow(10, DECIMAL);
    int ax = (int)(a->x() * multiple);
    int bx = (int)(b->x() * multiple);
    int ay = (int)(a->y() * multiple);
    int by = (int)(b->y() * multiple);
    int az = (int)(a->z() * multiple);
    int bz = (int)(b->z() * multiple);

    if (ax < bx){
        return -1;
    }
    else if (ax > bx){
        return 1;
    }
    else{
        if (ay < by)
        {
            return -1;
        }
        else if (ay > by)
        {
            return 1;
        }
        else{
            if (az < bz)
            {
                return -1;
            }
            else if (az > bz)
                return 1;
            else
                return 0;
        }
    }
}

/**
 * 오름차순
 * @param a
 * @param b
 * @return
 */
bool VertexComputation::greater(Vertex * a, Vertex * b) {
    ll multiple = pow(10, DECIMAL);
    int ax = (int)(a->x() * multiple);
    int bx = (int)(b->x() * multiple);
    int ay = (int)(a->y() * multiple);
    int by = (int)(b->y() * multiple);
    int az = (int)(a->z() * multiple);
    int bz = (int)(b->z() * multiple);

    if (ax < bx){
        return true;
    }
    else if (ax > bx){
        return false;
    }
    else{
        if (ay < by)
        {
            return true;
        }
        else if (ay > by)
        {
            return false;
        }
        else{
            if (az < bz)
            {
                return true;
            }
            else if (az > bz)
                return false;
            else
                return false;
        }
    }
}
