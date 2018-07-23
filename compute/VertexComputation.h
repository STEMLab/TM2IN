//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_VERTEXCOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_VERTEXCOMPUTATION_H

#include "detail/cgal_config.h"
#include "features/Geometry.h"

class VertexComputation {
public:
    static int compare(Vertex*,Vertex*);
    static bool greater(Vertex*, Vertex*);
};


#endif //TRIANGLEMESHTOCLEARSOLID_VERTEXCOMPUTATION_H
