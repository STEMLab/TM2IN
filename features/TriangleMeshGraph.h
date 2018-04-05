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
    vector<Triangle*> triangles;
    vector<vector<ull>> adjList;
    ull sizeOfTriangles = 0;

    TriangleMeshGraph(vector<Triangle*>& p_triangles){
        this->triangles = p_triangles;
    }

    void makeAdjacentGraph();
    bool isClosedTriangleMesh();
    bool isNeighbor(ull id1, ull id2);

    vector<ull> bfs(ull src, vector<bool>& checked);
    vector<vector<ull>> getConnectedComponent();
    vector<ull> getNeighbors(ull i);

    void clear(){
        if (!adjList.empty()) adjList.clear();
    }

    bool isEmpty();
};

#endif // SURFACEGRAPH_H_INCLUDED
