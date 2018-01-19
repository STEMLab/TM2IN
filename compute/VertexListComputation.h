//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H

#include "features/Vertex.h"

using namespace std;

class VertexListComputation {
public:
    static void deleteVertexList(vector<Vertex*>& vertices);

    static Vertex *getCenter(vector<Vertex *> vector);
};


#endif //TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H
