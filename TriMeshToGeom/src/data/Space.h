#ifndef TRIANGLESPACE_H
#define TRIANGLESPACE_H

#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <queue>

#include "predefine.h"

#include "data/SurfaceGraph.h"
#include "model/Triangle.h"
#include "logic/TriangleAttacher.h"
#include "data/Surface.hpp"
#include "data/Solid.h"
#include "logic/CleanPolygonMaker.h"
#include "model/Segment.h"

using namespace std;

class Space
{
public:
    vector<Vertex*> *p_vertexList;
    double min_coords[3];
    double max_coords[3];

    string name;
    double whole_area = 0;
    Space();
    Space(string pname, Checker* check);
    virtual ~Space();
    std::vector<Triangle> triangles;
    std::vector<Surface*> surfacesList;
    Solid* solid;
    SurfaceGraph* surface_graph;

    void pushTriangle(Triangle tri);

    int makeSurfacesBySeparation();
    int makeSurfacesByCandidator();
    int makeSurfacesGreedy(double degree);
    int makeSurfacesNotJoin();
    int checkTriangles();
    int checkOpposite();

    int combineSurface(double degree);
    int combineSurfaceByArea(double );
    int simplifySegment();
    int match00();
    int handleDefect();
    int updateNormal();

    int removeSurfacesNotConnectedFC();
    int removeOppositeSurfaces();
    int makeSurfacesPlanarWithLowest();

    int makeGraph();
    int remainOnlyUsingVertices();

    void tagID();
    int makeWallRectangle();
    int makeClosedWall();

    int makeFloorAndCeiling();
    void getVertexList(vector<Vertex*>& vt_list);

    Surface* findFirstSurfaceSimilarWithAxis(int axis);
    int findFirstSurfaceIndexSimilarWithAxis(int axis);
    int removeFloorAndCeiling();
    void rotateSpaceByFloorTo00();
    vector<Surface*> getTopSurfaces(double percent);

    /**
    * TODO
    */
    int handleTiltedWalls();

    //TODO : removed
    void test();

protected:
    void updateMBB();
    void freeSurfaces();
    void printProcess(ull, ull);
    Surface* attachTriangle(vector<Triangle*> tri_list, Surface* cp, bool* checked, ll& count, double degree);
    Surface* attachSurfaces(Surface* cp, ull start, bool* checked, ll& count, double degree);
    Surface* attachSurfacesByArea(Surface* cp, ull start, bool* checked, ll& count, double degree);
    vector<Surface*> makeSurfacesInList(vector<Triangle*>& tri_list, bool* checked, int& combined_count, double degree);
    vector<vector<Triangle*>> separateByNormal_6(vector<Triangle>& triangles);

    Surface* makeNewSurface(Segment* seg, double base, double height);
    vector<Surface*> clippingSurfaces(vector<Surface*>& walls);
    vector<Surface*> getWallsAndRemoveInSurfacesList(vector<Surface*>& walls);
    vector<Segment*> cutIntersection(vector<Segment*>& walls_2d);
    int countTheNumberOfVertex(vector<Segment*>& lines);
    bool connectWall_dfs(int , vector<vector<int> >& , vector<Segment*>& , vector<bool>&);
private:
    bool isIntersectIn(Segment* segment, vector<Segment*>& lines);
    static bool comparePairSegment(pair<int,Segment*>& a, pair<int,Segment*>& b);
    Checker* checker;
};


#endif // TRIANGLESPACE_H
