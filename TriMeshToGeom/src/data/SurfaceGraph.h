#ifndef SURFACEGRAPH_H_INCLUDED
#define SURFACEGRAPH_H_INCLUDED

#include "data/Surface.hpp"
#include "logic/util.h"
#include <queue>

using namespace std;

typedef pair<int, int> ii;

class SurfaceGraph{
public:
    ull size;

    vector<vector<ull> > adjList;
    void makeAdjacentGraph(vector<Surface*>& surface_list);
    void print_bfs();
    bool isNeighbor(ull id1, ull id2);
};

#endif // SURFACEGRAPH_H_INCLUDED
