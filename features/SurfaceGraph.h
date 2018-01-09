#ifndef SURFACEGRAPH_H_INCLUDED
#define SURFACEGRAPH_H_INCLUDED

#include "features/Surface.hpp"
#include "util.h"
#include <queue>

using namespace std;

typedef pair<int, int> ii;

class SurfaceGraph{
public:
    ull size;

    vector<vector<ull> > adjList;
    void makeAdjacentGraph(vector<Surface*>& surface_list);
    void makeAdjacentGraph(vector<Triangle>& tri_list);
    void attachNewTriagle(vector<Triangle>& tri_list);

    bool isClosedTriangleMesh();
    void print_bfs();
    bool isNeighbor(ull id1, ull id2);
};

#endif // SURFACEGRAPH_H_INCLUDED
