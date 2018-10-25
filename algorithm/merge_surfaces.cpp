//
// Created by dongmin on 18. 7. 19.
//

#include "algorithm/merge_surfaces.h"

#include <detail/cgal/geometry.h>
#include <detail/features/halfedge_string.h>
#include "detail/algorithm/merge_surfaces.h"
#include "detail/algorithm/simplify_share_edges.h"
#include "features/Wall/TriangulatedSurface.h"
#include "features/RoomBoundary/TriangulatedSurfaceMesh.h"

using namespace TM2IN::detail::algorithm;
using namespace TM2IN::RoomBoundary;

namespace TM2IN{
    namespace algorithm{
        bool mergeSurfaces(TriangulatedSurfaceMesh *tsm, double thres1, double thres2) {
            vector<Wall::TriangulatedSurface*> new_poly_list;
            SurfaceMerger sm(thres1, thres2);
            bool hasMerged = sm.mergeSurfaces(tsm->surface_list(), new_poly_list);
            tsm->setSurfacesList(new_poly_list);
            return hasMerged;
        }

        int cleanMergedSurfaces(TriangulatedSurfaceMesh *tsm) {
            bool hasSimplified = false;
            cout << "\n------------clean_merging_result------------\n" << endl;
            sort(tsm->surfaces.begin(), tsm->surfaces.end(), Surface::compareLength);
            ull sizeOfSurfaces = tsm->surfaces.size();

            for (ull i = 0 ; i < sizeOfSurfaces; i++)
                assert((int) tsm->surfaces[i]->getVerticesSize() >= 3);

            for (ull i = 0 ; i < sizeOfSurfaces - 1; i++){
                Wall::TriangulatedSurface *&surfaceI = tsm->surfaces[i];
                if (!surfaceI->easy_validation()) continue;
                printProcess(i, sizeOfSurfaces, "");
                for (ull j = i + 1; j < sizeOfSurfaces ; j++){
                    Wall::TriangulatedSurface *&surfaceJ = tsm->surfaces[j];
                    if (!surfaceI->easy_validation()) break;
                    if (!surfaceJ->easy_validation()) continue;
                    if (!TM2IN::detail::cgal::has_bbox_intersect(surfaceI, surfaceJ)) continue;
                    while (simplify_share_edges(tsm->surfaces[i], tsm->surfaces[j]) == 0){
                        hasSimplified = true;
                        if (!surfaceI->easy_validation() || !surfaceJ->easy_validation()) break;
                    }
                }
            }
            sizeOfSurfaces = tsm->surfaces.size();

            bool hasRemoved = false;
            for (int i = sizeOfSurfaces - 1; i >= 0 ; i--){
                if (tsm->surfaces[i]->strict_validation()){
                }
                else{
                    hasRemoved = true;
                    cerr << "remove" << endl;
                    TM2IN::detail::HalfEdgeString::setParent(tsm->surfaces[i]->getExteriorBoundary(), NULL);
                    delete tsm->surfaces[i];
                    tsm->surfaces.erase(tsm->surfaces.begin() + i);
                }
            }

            if (hasRemoved)
                hasSimplified = (cleanMergedSurfaces(tsm) || hasSimplified);

            return hasSimplified;
        }

        bool mergeSurfaces(std::vector<Wall::TriangulatedSurface *>& surfaceList, double thres1, double thres2, vector<Wall::TriangulatedSurface*>& newSurfaceList) {
            SurfaceMerger sm(thres1, thres2);
            bool hasMerged = sm.mergeSurfaces(surfaceList, newSurfaceList);
            return hasMerged;
        }

        bool mergeTriangles(vector<Wall::Triangle *> &triangleList, double thres1, double thres2,
                            vector<Wall::TriangulatedSurface *> &newSurfaceList) {
            vector<Wall::TriangulatedSurface *> surfaceList;
            for (Wall::Triangle* tri : triangleList)
                surfaceList.push_back(new Wall::TriangulatedSurface(tri));
            SurfaceMerger sm(thres1, thres2);
            bool hasMerged = sm.mergeSurfaces(surfaceList, newSurfaceList);
            for(Wall::TriangulatedSurface* sf : surfaceList)
                delete sf;
            return hasMerged;
        }


    }
}