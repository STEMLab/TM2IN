#ifndef TRIANGLESPACE_H
#define TRIANGLESPACE_H

#include <string>
#include <vector>
#include <bitset>

#include "Trinagle.h"
#include "CombinedPolygon.hpp"

using namespace std;

class TriangleSpace
{
public:
    string name;
    TriangleSpace();
    virtual ~TriangleSpace();
    std::vector<Triangle> triangles;
    std::vector<CombinedPolygon*> polygon_list;

    TriangleSpace(string pname) {name = pname;}
    int makePolygonsBySeparation(Checker* checker);
    int makePolygonsByCandidator(Checker* checker);
    int makePolygonsGreedy(Checker* checker);

    int makePolygonsCoplanar(Checker* checker);
    int combineCombinedPolygon(Checker* checker);
    void pushTriangle(Triangle tri);
protected:
    void printProcess(ull, ull);
    CombinedPolygon* attachTriangle(vector<Triangle*> tri_list, CombinedPolygon* cp, bool* checked, int& id, Checker* check);
    vector<CombinedPolygon*> makePolygonsInList(vector<Triangle*>& tri_list, bool* checked, Checker* check, int& combined_count);
    vector<vector<Triangle*>> separateByNormal_6(vector<Triangle>& triangles);
private:

};

#endif // TRIANGLESPACE_H
