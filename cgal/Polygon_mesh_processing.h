//
// Created by dongmin on 18. 1. 17.
//

#ifndef POLYGON_MESH_PROCESSING_H
#define POLYGON_MESH_PROCESSING_H

#include <iostream>
#include <vector>
#include "features/Vertex.h"
#include "features/Surface.h"

using namespace std;
using namespace TM2IN;

namespace TM2IN{
    namespace unused{
        vector<Vertex *> fillHole(vector<Vertex*>& vertices, vector<Surface *>& surfaces);

    }
};


#endif //POLYGON_MESH_PROCESSING_H
