#ifndef TRIANGLESPACE_H
#define TRIANGLESPACE_H

#include <string>
#include <vector>
#include <bitset>

#include "model/Trinagle.h"
#include "logic/TriangleAttacher.h"
#include "data/Surface.hpp"
#include "logic/CleanPolygonMaker.h"

using namespace std;

class TriangleSpace
{
public:
    string name;
    TriangleSpace();
    TriangleSpace(string pname, Checker* check);
    virtual ~TriangleSpace();
    std::vector<Triangle> triangles;
    std::vector<Surface*> polygon_list;


    int makeSurfacesBySeparation();
    int makeSurfacesByCandidator();
    int makeSurfacesGreedy();
    int makeSurfacesNotJoin();

    int makeSurfacesCoplanar();
    int combineSurface();
    int simplifySegment();

    void pushTriangle(Triangle tri);
protected:
    void freeSurfaces();
    void printProcess(ull, ull);
    Surface* attachTriangle(vector<Triangle*> tri_list, Surface* cp, bool* checked, ll& count);
    Surface* attachSurfaces(Surface* cp, ull start, bool* checked, ll& count);
    vector<Surface*> makeSurfacesInList(vector<Triangle*>& tri_list, bool* checked, int& combined_count);
    vector<vector<Triangle*>> separateByNormal_6(vector<Triangle>& triangles);
private:
    Checker* checker;
};

#endif // TRIANGLESPACE_H
