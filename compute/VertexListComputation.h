//
// Created by dongmin on 18. 1. 18.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H

#include "features/Vertex.h"
#include "detail/cgal_config.h"

using namespace std;

class VertexListComputation {
public:
    static void deleteVertexList(vector<Vertex*>& vertices);

    static Vertex *getCenter(vector<Vertex *> vector);
    static bool checkDuplicate(vector<Vertex*> vertices);
};


#endif //TRIANGLEMESHTOCLEARSOLID_VERTEXLISTCOMPUTATION_H
