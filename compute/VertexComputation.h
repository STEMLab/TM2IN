//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_VERTEXCOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_VERTEXCOMPUTATION_H

#include "cgal/Types.h"
#include "features/Vertex.h"

class VertexComputation {
public:
    static Point_3 getCGALPoint(Vertex* vt);
    static int compare(Vertex*,Vertex*);
    static bool greater(Vertex*, Vertex*);
};


#endif //TRIANGLEMESHTOCLEARSOLID_VERTEXCOMPUTATION_H
