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
#include "logic/CleanPolygonMaker.h"
#include "model/Segment.h"

using namespace std;

class Space
{
public:
    Space();
    Space(string pname, Checker* check);
    virtual ~Space();

    std::vector<Triangle> triangles;
    std::vector<Surface*> surfacesList;
    vector<Vertex*> *p_vertexList;

    double min_coords[3];
    double max_coords[3];
    Checker* checker;
    string name;
    double whole_area = 0;
    SurfaceGraph* surface_graph;

    void pushTriangle(Triangle tri);

    int makeSurfacesBySeparation();
    int makeSurfacesByCandidator();
    int mergeTrianglesGreedy(double degree);
    int makeSurfacesNotJoin();

    int checkTriangles();
    int checkOpposite();

    int combineSurface(double degree);
    int simplifySegment();
    int match00();
    int handleDefect();
    int updateNormal();

    int combineSurfaceMoreGreedy();

    int remainOnlyUsingVertices();

    void rotateSpaceByFloorTo00();
    vector<Surface*> getTopSurfaces(double percent);

    SimpleSpace* makeSimpleSpace();
    //TODO : removed
    void test();

protected:
    void updateMBB();
    void freeSurfaces();
    Surface* attachTriangle(vector<Triangle*> tri_list, Surface* cp, bool* checked, ll& count, double degree);
    Surface* attachSurfaces(Surface* cp, ull start, bool* checked, ll& count, double degree);
    vector<Surface*> makeSurfacesInList(vector<Triangle*>& tri_list, bool* checked, int& combined_count, double degree);

private:

};


#endif // TRIANGLESPACE_H
