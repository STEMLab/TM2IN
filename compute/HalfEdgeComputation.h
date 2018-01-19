//
// Created by dongmin on 18. 1. 19.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_HALFEDGECOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_HALFEDGECOMPUTATION_H


#include <cgal/Types.h>
#include <features/HalfEdge.h>

class HalfEdgeComputation {
public:
    static Line_3 getCGALLine(HalfEdge *he);
};


#endif //TRIANGLEMESHTOCLEARSOLID_HALFEDGECOMPUTATION_H
