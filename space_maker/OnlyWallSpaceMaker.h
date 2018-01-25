#ifndef ONLYWALLSPACEMAKER_H_INCLUDED
#define ONLYWALLSPACEMAKER_H_INCLUDED

#include "predefine.h"
#include "compute/SurfacesListComputation.h"

using namespace std;

class OnlyWallSpaceMaker{
public:
    Checker* checker;
    OnlyWallSpaceMaker(){};

    vector<Surface*> makeSimpleSurfaces(vector<Surface*> surfacesList);
private:

    static Surface* findFirstSurfaceSimilarWithAxis(vector<Surface*>& surfacesList, int axis);

    int removeSurfacesNotConnectedFC(vector<Surface*>& surfacesList, SurfaceGraph*);
    int removeOppositeSurfaces(vector<Surface*>& surfacesList);
    int makeSurfacesPlanarWithLowest(vector<Surface*>& surfacesList);
    int makeWallRectangle(vector<Surface*>& surfacesList);
    int makeClosedWall(vector<Surface*>& surfacesList);
    int removeFloorAndCeiling(vector<Surface*>& surfacesList);
    int makeFloorAndCeiling(vector<Surface*>& surfacesList);

    Surface* makeNewSurface(Segment* seg, double base, double height);
    vector<Surface*> clippingSurfaces(vector<Surface*>& walls);
    vector<Surface*> getWallsAndRemoveInSurfacesList(vector<Surface*>& surfacesList, vector<Surface*>& walls);
    vector<Segment*> cutIntersection(vector<Segment*>& walls_2d);
    bool connectWall_dfs(int , vector<vector<int> >& , vector<Segment*>& , vector<bool>&);

    bool isIntersectIn(Segment* segment, vector<Segment*>& lines);
    static bool comparePairSegment(pair<int,Segment*>& a, pair<int,Segment*>& b);

    Segment* makeSegmentLowerZ(Surface* sf, Checker* ch);
    Segment* makeSegmentUpperZ(Surface* sf, Checker* ch);

    void updateRectArea(Surface* sf);
};

#endif // ONLYWALLSPACEMAKER_H_INCLUDED
