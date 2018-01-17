#ifndef TRIANGLESPACE_H
#define TRIANGLESPACE_H

#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <queue>

#include "predefine.h"

#include "features/SurfaceGraph.h"
#include "features/Triangle.h"
#include "features/Surface.hpp"
#include "computation/SurfacePairComputation.h"
#include "features/Segment.h"

using namespace std;

class Space
{
public:
    Space();
    Space(string pname, Checker* check);
    virtual ~Space();

    std::vector<Surface*> surfacesList;
    vector<Vertex*> *p_vertexList;

    double min_coords[3];
    double max_coords[3];
    Checker* checker;
    string name;
    double whole_area = 0;
    SurfaceGraph* surface_graph;

    void setName(string _name){
        name = _name;
    }
    void setChecker(Checker* ch){
        checker = ch;
    }
    int convertTrianglesToSurfaces(vector<Triangle*>& triangles);
    int checkDuplicateVertexInSurfaces();

    int combineSurface(double degree);
    int simplifySegment();
    int match00();
    int handleDefect(double angle);
    int updateNormal();

    int snapSurface(double);
    void rotateSpaceByFloorTo00();

    //TODO : removed
    void test();

protected:
    Surface* attachSurfaces(Surface* cp, ull start, bool* checked, ll& count, double degree);
    void updateMBB();
    void freeSurfaces();
private:

};


#endif // TRIANGLESPACE_H
