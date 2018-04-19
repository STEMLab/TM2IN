#ifndef CleanPolygonMaker_H_INCLUDED
#define CleanPolygonMaker_H_INCLUDED

#include <vector>
#include <map>

#include "cgal/CGALCalculation.h"
#include "features/Surface.h"
#include "util.h"

using namespace std;
namespace TMIC {
    int findStartAndEnd(vector<Vertex*>& vi, vector<Vertex*>& vj, ll middle_i, ll middle_j, ll& start_i, ll& end_i, ll& start_j, ll& end_j);
    bool findShareVertex(vector<Vertex*>& vi, vector<Vertex*>& vj, ll& middle_i, ll& middle_j);
    bool isNeighbor(Surface *origin, Surface *piece);
    int doShareEdge(Surface *&vi, Surface *&vj);

    vector<int> listShareEdgesInPiece(Surface *origin, Surface *piece);
    vector<int> listShareEdgesInOrigin(Surface *origin, Surface *piece, vector<int> vector1);

    int combine(Surface *cp1, Surface *cp2);
    int simplifyLineSegment(Surface *origin, Surface *piece);
};

#endif // CleanPolygonMaker_H_INCLUDED
