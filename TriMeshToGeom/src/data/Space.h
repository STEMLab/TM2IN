#ifndef TRIANGLESPACE_H
#define TRIANGLESPACE_H

#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <queue>

#include "data/SurfaceGraph.h"
#include "model/Triangle.h"
#include "logic/TriangleAttacher.h"
#include "data/Surface.hpp"
#include "data/Solid.h"
#include "logic/CleanPolygonMaker.h"

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

    int makeSurfacesBySeparation();
    int makeSurfacesByCandidator();
    int makeSurfacesGreedy(double degree);
    int makeSurfacesNotJoin();

    int combineSurface(double degree);
    int combineSurfaceByArea(double );
    int simplifySegment();
    int match00();
    int handleDefect();
    void tagID();
    int makeSurfacesPlanar(Checker*);
    int remainOnlyUsingVertexes();
    int updateNormal();
    int makeGraph(Checker* ch);
    void pushTriangle(Triangle tri);

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
private:
    Checker* checker;
};

#endif // TRIANGLESPACE_H
