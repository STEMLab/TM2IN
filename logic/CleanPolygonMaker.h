#ifndef CleanPolygonMaker_H_INCLUDED
#define CleanPolygonMaker_H_INCLUDED

#include <vector>
#include <map>

#include "logic/CGALCalculation.h"
#include "features/Surface.hpp"
#include "util.h"

using namespace std;
class CleanPolygonMaker{
private:
    static bool isNeighbor(Surface* cp1, Surface* cp2);
    static int findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j);
    static vector<Vertex*> simplifySegment(vector<Vertex*>& origin, ll start, ll end, Checker* checker);
    static bool isMakingHole(ll start_i, ll end_i, ll start_j, ll end_j , vector<Vertex*>& piece_v_list, vector<Vertex*>& origin_v_list);
public:
    static int combine(Surface* cp1, Surface* cp2, Checker* checker, double degree);
    static bool findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j);
    static bool isShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj);
    static int simplifyLineSegment(Surface* origin, Surface*, bool again);
};

#endif // CleanPolygonMaker_H_INCLUDED
