#ifndef TriangleMeshGraph_h
#define TriangleMeshGraph_h

#include "predefine.h"
#include "features/Graph.h"
#include "features/TriangleMesh.h"
#include "util.h"
#include <queue>

using namespace std;


class TriangleMeshGraph : public Graph{
public:
    void makeAdjacentGraph(TriangleMesh* tm);

    bool isClosedTriangleMesh();
    void print_bfs();
    bool isNeighbor(ull id1, ull id2);
};

#endif // SURFACEGRAPH_H_INCLUDED
