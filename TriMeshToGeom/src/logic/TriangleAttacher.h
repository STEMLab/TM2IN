#ifndef TRIANGLEATTACHER_H_INCLUDED
#define TRIANGLEATTACHER_H_INCLUDED

#include "data/CombinedPolygon.hpp"

class TriangleAttacher
{
public:
    static bool attach(CombinedPolygon* cp, Triangle* pl, Checker* ch);
    static bool isNeighbor(CombinedPolygon* cp, Triangle* pl);
    static bool checkMakeHole(CombinedPolygon* cp, ll index, Vertex* add_id);
    static int isShareTwoLine(CombinedPolygon* cp, ll index, Vertex* add_id);
    static ll findShareLine(CombinedPolygon* cp, Triangle* pl, Checker* ch, Vertex** add);
    static bool isShareThreeLine(CombinedPolygon* cp, ll index);

};
#endif // TRIANGLEATTACHER_H_INCLUDED
