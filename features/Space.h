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
#include "compute/Surface_pair_computation.h"

using namespace std;

class Space
{
public:
    Space();
    Space(string pname);
    virtual ~Space();

    std::vector<Surface*> surfacesList;
    vector<Vertex *> vertices;
    SurfaceGraph* surfaceGraph;

    double min_coords[3];
    double max_coords[3];
    string name;
    double whole_area = 0;
    int generation;
    bool hasTriangulation = false;

    void setName(string _name){
        name = _name;
    }
    int convertTrianglesToSurfaces(vector<Triangle*>& triangles);
    int checkDuplicateVertexInSurfaces();

    int combineSurface();
    int simplifySegment();
    int checkSurfaceValid();
    int updateNormal();

    void sortSurfacesByArea();

    void tagID();

    void triangulateSurfaces();

    int checkSelfIntersection();

    vector<Triangle *> getTriangleListOfAllSurfaces();

    int removeStraight();


    int translateSpaceToOrigin();
    int snapSurface(double);
    void rotateSpaceByFloorTo00();
    void resolveIntersectionINTRASurface();
    void clearTrianglesListInSurfaces();
    int makeSurfacesPlanar();

protected:
    Surface *attachSurfaces(Surface *cp, ull start, bool *checked, ll &count);
    void updateMBB();
    void freeSurfaces();


};


#endif // SPACE_H
