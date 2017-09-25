#ifndef SURFACEGRAPH_H_INCLUDED
#define SURFACEGRAPH_H_INCLUDED

#include "data/Surface.hpp"
#include "logic/util.h"

using namespace std;

typedef pair<int, int> ii;

class SurfaceGraph{
public:
    vector<vector<pair<ull, ii>>> adjList;
    void makeAdjacentGraph(vector<Surface*>& surface_list);

};

#endif // SURFACEGRAPH_H_INCLUDED
