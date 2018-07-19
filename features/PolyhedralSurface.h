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

class PolyhedralSurface
{
public:
    PolyhedralSurface();
    PolyhedralSurface(string pname);
    virtual ~PolyhedralSurface();

    std::vector<Surface*> surfacesList;
    vector<Vertex *> vertices;
    SurfaceGraph* surfaceGraph;

    CGAL::Bbox_3 mbb;
    string name;
    double whole_area = 0;
    int generation;

    void setName(string _name){
        name = _name;
    }
    int convertTrianglesToSurfaces(vector<Triangle*>& triangles);
    int checkDuplicateVertexInSurfaces();

    int mergeSurface();
    int simplifySegment();
    int checkSurfaceValid();
    int updateNormal();

    void sortSurfacesByArea();

    void tagID();

    void triangulateSurfaces();

    int checkSelfIntersection();

    vector<Triangle *> getTriangulation();

    int removeStraight();
protected:
    void updateMBB();
    void freeSurfaces();

};


#endif // SPACE_H
