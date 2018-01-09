#ifndef SURFACESLISTCALCULATION_H_INCLUDED
#define SURFACESLISTCALCULATION_H_INCLUDED

#include "predefine.h"
#include "features/Surface.hpp"

using namespace std;

class SurfacesListCalculation{
public:
    static Surface* findFirstSurfaceSimilarWithAxis(vector<Surface*>& surfacesList, int axis);
    static int findFirstSurfaceIndexSimilarWithAxis(vector<Surface*>& surfacesList, int axis);
    static SurfaceGraph* makeGraph(vector<Surface*>& surfacesList);
    static void tagID(vector<Surface*>& surfacesList);
    static void getMBB(vector<Surface*>& surfacesList, vector<vector<double>>& v);
};

#endif // SURFACESLISTCALCULATION_H_INCLUDED
