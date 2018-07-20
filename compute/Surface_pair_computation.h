#ifndef CleanPolygonMaker_H_INCLUDED
#define CleanPolygonMaker_H_INCLUDED

#include <vector>
#include <map>

#include "cgal/CGALCalculation.h"
#include "features/Surface.h"
#include "util.h"

using namespace std;
namespace TMIC {
    int doShareEdge(Surface *&vi, Surface *&vj);

    vector<int> listShareEdgesInPiece(Surface *origin, Surface *piece);
    vector<int> listShareEdgesInOrigin(Surface *origin, Surface *piece, vector<int> vector1);
};

#endif // CleanPolygonMaker_H_INCLUDED
