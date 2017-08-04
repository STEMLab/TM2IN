#ifndef POLYGONCOMBINER_H_INCLUDED
#define POLYGONCOMBINER_H_INCLUDED

#include <vector>

#include "data/CombinedPolygon.hpp"

using namespace std;
class PolygonCombiner{
public:
    static bool combine(CombinedPolygon* cp1, CombinedPolygon* cp2, Checker* checker);
    static bool isNeighbor(CombinedPolygon* cp1, CombinedPolygon* cp2);
};

#endif // POLYGONCOMBINER_H_INCLUDED
