//
// Created by dongmin on 18. 1. 18.
//

#include "VertexComputation.h"

Point_3 VertexComputation::getCGALPoint(Vertex *v) {
    return Point_3(v->x(),v->y(),v->z());;
}
