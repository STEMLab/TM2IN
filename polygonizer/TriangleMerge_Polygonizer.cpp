//
// Created by dongmin on 18. 4. 23.
//

#include <compute/SurfacesListComputation.h>
#include <compute/Connect_halfedges.h>
#include "TriangleMerge_Polygonizer.h"

void TriangleMerge_Polygonizer::make(Space *space) {
    assert(space->hasTriangulation);
    Checker::merge_degree = 45.00;
    Checker::coplanar_degree = 1.0;
    vector<Surface*> newSurfacesList;
    for (int i = 0 ; i < space->surfacesList.size() ; i++){
        Surface* sf = space->surfacesList[i];
        TMIC::connectOppositeHalfEdges(sf->triangles);
        vector<Surface*> newSurfacesInSurface = TMIC::mergeSurfaces(sf->triangles);
        newSurfacesList.insert(newSurfacesList.end(), newSurfacesInSurface.begin(), newSurfacesInSurface.end());
    }
    space->surfacesList = newSurfacesList;
    cout << "Surfaces : " << space->surfacesList.size() << endl;
    return ;
}
