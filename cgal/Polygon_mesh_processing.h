//
// Created by dongmin on 18. 1. 17.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_SURFACEHOLECOVER_H
#define TRIANGLEMESHTOCLEARSOLID_SURFACEHOLECOVER_H

#include <iostream>
#include <vector>
#include "features/Vertex.h"
#include "features/Surface.h"
#include "features/TriangleMesh.h"

using namespace std;
using namespace TM2IN;

/**
 * @todo will be removed
 *
 */

namespace CGAL_User{
    vector<Vertex *> fillHole(vector<Vertex*>& vertices, vector<Surface *>& surfaces);
    void fillHole(TriangleMesh* tm);
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACEHOLECOVER_H
