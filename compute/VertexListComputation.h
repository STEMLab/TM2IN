//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H

#include "features/Vertex.h"
#include "cgal/Types.h"

using namespace std;

class VertexListComputation {
public:
    static void deleteVertexList(vector<Vertex*>& vertices);

    static Vertex *getCenter(vector<Vertex *> vector);
    static Plane_3 getPlane3WithPCA(std::vector<Vertex *> vector);
    static vector<HalfEdge*> makeHalfEdgesList(vector<Vertex*> vector);

    static vector<Segment_2> makeSegment2List(vector<Point_2>& pointsList);
};


#endif //TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H
