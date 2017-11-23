#ifndef TRIANGLEATTACHER_H_INCLUDED
#define TRIANGLEATTACHER_H_INCLUDED

#include "model/Surface.hpp"

using namespace std;

class TriangleCalculation
{
public:
    static bool attach(Surface* cp, Triangle* pl, Checker* ch, double degree);
    static vector<Surface*> clusterAndmakeSurfaces(vector<Triangle>& tri_list);
private:
    static bool isNeighbor(Surface* cp, Triangle* pl);
    static bool checkMakeHole(Surface* cp, ll index, Vertex* add_id);
    static int isShareTwoLine(Surface* cp, ll index, Vertex* add_id);
    static ll findShareLine(Surface* cp, Triangle* pl, Vertex** add);
    static bool isShareThreeLine(Surface* cp, ll index);

};
#endif // TRIANGLEATTACHER_H_INCLUDED
