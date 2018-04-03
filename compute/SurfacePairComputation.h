#ifndef CleanPolygonMaker_H_INCLUDED
#define CleanPolygonMaker_H_INCLUDED

#include <vector>
#include <map>

#include "cgal/CGALCalculation.h"
#include "features/Surface.h"
#include "util.h"

using namespace std;
class SurfacePairComputation{
private:
    static int findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j);
    static vector<Vertex*> simplifySegment(vector<Vertex*>& origin, ll start, ll end);
    static bool isMakingHole(ll start_i, ll end_i, ll start_j, ll end_j , vector<Vertex*>& piece_v_list, vector<Vertex*>& origin_v_list);

    // intersect

public:
    static int combine(Surface* cp1, Surface* cp2, double degree);
    static bool findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j);
    static int simplifyLineSegment(Surface *origin, Surface *);

    static bool doShareEdge(Surface *&vi, Surface *&vj);

    static bool CanBeNeighbor(Surface *cp1, Surface *cp2);

    static bool isNeighbor(Surface *origin, Surface *piece);

    static vector<int> listShareEdgesInPiece(Surface *origin, Surface *piece);

    static vector<int> listShareEdgesInOrigin(Surface *origin, Surface *edges, vector<int> vector1);

};

#endif // CleanPolygonMaker_H_INCLUDED
