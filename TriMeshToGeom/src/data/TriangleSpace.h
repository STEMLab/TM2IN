#ifndef TRIANGLESPACE_H
#define TRIANGLESPACE_H

#include <string>
#include <vector>
#include <bitset>

#include "model/Trinagle.h"
#include "logic/TriangleAttacher.h"
#include "data/CombinedPolygon.hpp"
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
    std::vector<CombinedPolygon*> polygon_list;


    int makePolygonsBySeparation();
    int makePolygonsByCandidator();
    int makePolygonsGreedy();

    int makePolygonsCoplanar();
    int combineCombinedPolygon();
    int simplifySegment();

    void pushTriangle(Triangle tri);
protected:
    void freeCombinedPolygons();
    void printProcess(ull, ull);
    CombinedPolygon* attachTriangle(vector<Triangle*> tri_list, CombinedPolygon* cp, bool* checked, ll& count);
    CombinedPolygon* attachPolygons(CombinedPolygon* cp, ull start, bool* checked, ll& count);
    vector<CombinedPolygon*> makePolygonsInList(vector<Triangle*>& tri_list, bool* checked, int& combined_count);
    vector<vector<Triangle*>> separateByNormal_6(vector<Triangle>& triangles);
private:
    Checker* checker;
};

#endif // TRIANGLESPACE_H
