#include "features/TriangleMesh.h"
#include <utility>

TriangleMesh* TriangleMesh::correct(){
    this->makeGraph();
}

void TriangleMesh::makeGraph(){
    this->graph = new TriangleMeshGraph();
    this->graph->makeAdjacentGraph(this);
}
