#ifndef SPACE_H
#define SPACE_H

#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <queue>

#include "predefine.h"

#include "features/SurfaceGraph.h"
#include "features/Triangle.h"
#include "features/Surface.h"
#include "compute/SurfacePairComputation.h"

using namespace std;

class Space
{
public:
    Space();
    Space(string pname);
    virtual ~Space();

    std::vector<Surface*> surfacesList;
    vector<Vertex*> *p_vertexList;

    double min_coords[3];
    double max_coords[3];
    string name;
    double whole_area = 0;
    bool hasTriangulation = false;

    void setName(string _name){
        name = _name;
    }
    int convertTrianglesToSurfaces(vector<Triangle*>& triangles);
    int checkDuplicateVertexInSurfaces();

    int combineSurface(double degree);
    int simplifySegment();
    int translateSpaceToOrigin();
    int checkSurfaceValid();
    int updateNormal();

    int snapSurface(double);
    void rotateSpaceByFloorTo00();

    int makeSurfacesPlanar();

    void sortSurfacesByArea();

    void tagID();

    void putVerticesAndUpdateIndex(vector<Vertex *> &vector);

    void resolveIntersectionINTRASurface();

    void clearTrianglesListInSurfaces();

    void triangulateSurfaces();

    int checkSelfIntersection();

    vector<Triangle *> getTriangleList();

protected:
    Surface* attachSurfaces(Surface* cp, ull start, bool* checked, ll& count, double degree);
    void updateMBB();
    void freeSurfaces();

};


#endif // SPACE_H
