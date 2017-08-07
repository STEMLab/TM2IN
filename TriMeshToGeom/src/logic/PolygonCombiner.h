#ifndef POLYGONCOMBINER_H_INCLUDED
#define POLYGONCOMBINER_H_INCLUDED

#include <vector>

#include "data/CombinedPolygon.hpp"

using namespace std;
class PolygonCombiner{
public:
    static bool combine(CombinedPolygon* cp1, CombinedPolygon* cp2, Checker* checker);
    static bool isNeighbor(CombinedPolygon* cp1, CombinedPolygon* cp2);
    static void findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j);
};

#endif // POLYGONCOMBINER_H_INCLUDED
