//
// Created by dongmin on 18. 1. 17.
//

#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <iostream>
#include <vector>
#include "features/IndoorComponent.h"

using namespace std;
using namespace TM2IN;

namespace TM2IN{
    namespace unused{
        /**
         * @brief Fills hole in surfaces
         * @ingroup unused
         */
        vector<Vertex *> fillHole(vector<Vertex*>& vertices, vector<Wall::Triangle *>& triangles);
    }
};


#endif //POLYHEDRON_H
