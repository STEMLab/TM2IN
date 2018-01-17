#ifndef TriangleMeshGraph_h
#define TriangleMeshGraph_h

#include "features/TriangleMesh.h"
#include "util.h"
#include <queue>
#include <utility>
#include <map>

using namespace std;


class TriangleMeshGraph{
private:
public:
    vector<vector<ull>> adjList;
    ull sizeOfTriangles = 0;

    void makeAdjacentGraph(vector<Triangle*>& triangles);

    bool isClosedTriangleMesh();
    bool isNeighbor(ull id1, ull id2);

    vector<ull> bfs(ull src, vector<bool>& checked);
    vector<vector<ull>> getConnectedComponent();

    bool canBeNeighbor(Triangle* t1, Triangle* t2);
    vector<ull> getNeighbors(ull i);
};

#endif // SURFACEGRAPH_H_INCLUDED
