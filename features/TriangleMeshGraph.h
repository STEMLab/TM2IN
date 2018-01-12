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
    ull sizeOfTriangles = 0;
    void makeAdjacentGraph(vector<Triangle>& triangles);

    bool isClosedTriangleMesh();
    bool isNeighbor(ull id1, ull id2);

    vector<ull> bfs(ull src, vector<bool>& checked);
    vector<vector<ull>> getConnectedComponent();
};


#endif
