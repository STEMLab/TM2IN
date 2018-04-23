//
// Created by dongmin on 18. 4. 23.
//

#include "Triangle_Polygonizer.h"

#include "compute/Connect_halfedges.h"
#include "features/HalfEdge.h"
#include "features/TriangleMeshGraph.h"

void Triangle_Polygonizer::make(Space *space) {
    assert(space->hasTriangulation);
    vector<Triangle*> triangleList = space->getTriangleListOfAllSurfaces();
    vector<Surface*> newSurfaceList;

    for (int i = 0 ; i < triangleList.size() ; i++){
        for (HalfEdge* he : triangleList[i]->boundaryEdges){
            assert(he->getOppositeEdge() == NULL);
        }
    }
    TMIC::connectOppositeHalfEdges(triangleList);
    TriangleMeshGraph* graph = new TriangleMeshGraph(triangleList);
    graph->makeAdjacentGraph();
    assert(graph->isClosedTriangleMesh());

    for (int i = 0 ; i < triangleList.size() ; i++){
        newSurfaceList.push_back(triangleList[i]);
    }

    space->surfacesList = newSurfaceList;
}
