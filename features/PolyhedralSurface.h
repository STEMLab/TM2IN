#ifndef SPACE_H
#define SPACE_H

#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <queue>

#include "predefine.h"

#include "features/Triangle.h"
#include "features/Surface.h"
using namespace std;

class PolyhedralSurface
{
public:
    PolyhedralSurface();
    PolyhedralSurface(string pname);
    virtual ~PolyhedralSurface();

    std::vector<Surface*> surfacesList;
    vector<Vertex *> vertices;

    CGAL::Bbox_3 mbb;
    string name;
    double whole_area = 0;
    int generation;

    void setName(string _name){
        name = _name;
    }
    int convertTrianglesToSurfaces(vector<Triangle*>& triangles);
    int checkDuplicateVertexInSurfaces();

    int checkSurfaceValid();

    /**
     * Check whether PolyhedralSurface is closed or not by bfs.
     * @return Boolean
     */
    bool isClosed();

    int updateNormal();
    void sortSurfacesByArea();
    void tagID();
    int checkSelfIntersection();
    string asJsonText();
    void setSurfacesList(vector<Surface *> new_list);

    int removeStraight();
protected:
    void updateMBB();
    void freeSurfaces();

};


#endif // SPACE_H
