//
// Created by dongmin on 18. 1. 17.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_SURFACEHOLECOVER_H
#define TRIANGLEMESHTOCLEARSOLID_SURFACEHOLECOVER_H

#include <features/Vertex.h>

using namespace std;

class SurfaceHoleCover {
public:
    static vector<Vertex *> fillHole(vector<Vertex*>& vertices, vector<Surface *>& surfaces) ;
};


#endif //TRIANGLEMESHTOCLEARSOLID_SURFACEHOLECOVER_H
