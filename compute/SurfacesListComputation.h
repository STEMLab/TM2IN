#ifndef SURFACESLISTCALCULATION_H_INCLUDED
#define SURFACESLISTCALCULATION_H_INCLUDED

#include "predefine.h"
#include "features/Surface.h"
#include "SurfacesListComputation.h"

using namespace std;

class SurfacesListComputation{
public:
    static int findFirstSurfaceIndexSimilarWithAxis(vector<Surface*>& surfacesList, int axis);
    static void tagID(vector<Surface*>& surfacesList);
    static vector<vector<double>> getMBB(vector<Surface*>& surfacesList);
    static ull countTriangles(vector<TriangleMesh*> tm);

};

namespace TMIC{
    vector<Surface*> mergeSurfaces(vector<Surface*>& surfacesList);
    vector<Surface*> mergeSurfaces(vector<Triangle*>& triangleList);
}

#endif // SURFACESLISTCALCULATION_H_INCLUDED
