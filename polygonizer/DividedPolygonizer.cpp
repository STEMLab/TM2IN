//
// Created by dongmin on 18. 4. 23.
//

#include <compute/SurfacesListComputation.h>
#include <compute/Connect_halfedges.h>
#include "DividedPolygonizer.h"

void DividedPolygonizer::make(Solid *space) {
    Checker::threshold_2 = 45.00;
    Checker::threshold_1 = 1.0;
    vector<Surface*> newSurfacesList;
    for (int i = 0 ; i < space->surfacesList.size() ; i++){
        Surface* sf = space->surfacesList[i];
        assert(sf->triangulation.size());

        TMIC::connectOppositeHalfEdges(sf->triangulation);
        vector<Surface*> newSurfacesInSurface;
        TMIC::mergeSurfaces(sf->triangulation, newSurfacesInSurface);
        newSurfacesList.insert(newSurfacesList.end(), newSurfacesInSurface.begin(), newSurfacesInSurface.end());
    }
    space->surfacesList = newSurfacesList;
    cout << "Surfaces : " << space->surfacesList.size() << endl;
    return ;
}
