//
// Created by dongmin on 18. 7. 19.
//

#include "algorithm/merge_surfaces.h"
#include "detail/algorithm/simplify_share_edges.h"
#include <features/PolyhedralSurface.h>

using namespace TM2IN::detail::algorithm;

namespace TM2IN{
    namespace algorithm{
        bool mergeSurfaces(PolyhedralSurface *ps, double thres1, double thres2) {
            vector<Surface*> new_poly_list;
            SurfaceMerger sm(thres1, thres2);
            bool hasMerged = sm.mergeSurfaces(ps->surfacesList, new_poly_list);
            ps->setSurfacesList(new_poly_list);
            return hasMerged;
        }

        int cleanMergedSurfaces(PolyhedralSurface *ps) {
            bool hasSimplified = false;
            cout << "\n------------clean_merging_result------------\n" << endl;
            sort(ps->surfacesList.begin(), ps->surfacesList.end(), Surface::compareLength);
            ull sizeOfSurfaces = ps->surfacesList.size();

            for (ull i = 0 ; i < sizeOfSurfaces; i++)
                assert((int) ps->surfacesList[i]->getVerticesSize() >= 3);

            for (ull i = 0 ; i < sizeOfSurfaces - 1; i++){
                Surface *&surfaceI = ps->surfacesList[i];
                if (!surfaceI->easy_validation()) continue;
                printProcess(i, sizeOfSurfaces, "");
                for (ull j = i + 1; j < sizeOfSurfaces ; j++){
                    Surface *&surfaceJ = ps->surfacesList[j];
                    if (!surfaceI->easy_validation()) break;
                    if (!surfaceJ->easy_validation()) continue;
                    if (!CGALCalculation::isIntersect_BBOX(surfaceI, surfaceJ)) continue;
                    while (simplify_share_edges(ps->surfacesList[i], ps->surfacesList[j]) == 0){
                        hasSimplified = true;
                        if (!surfaceI->easy_validation() || !surfaceJ->easy_validation()) break;
                    }
                }
            }
            sizeOfSurfaces = ps->surfacesList.size();

            bool hasRemoved = false;
            for (int i = sizeOfSurfaces - 1; i >= 0 ; i--){
                if (ps->surfacesList[i]->strict_validation()){
                }
                else{
                    hasRemoved = true;
                    cout << "remove" << endl;
                    delete ps->surfacesList[i];
                    ps->surfacesList.erase(ps->surfacesList.begin() + i);
                }
            }

            if (hasRemoved)
                hasSimplified = (cleanMergedSurfaces(ps) || hasSimplified);

            return hasSimplified;
        }

        bool mergeSurfaces(vector<Surface *>& surfaceList, double thres1, double thres2, vector<Surface*>& newSurfaceList) {
            SurfaceMerger sm(thres1, thres2);
            bool hasMerged = sm.mergeSurfaces(surfaceList, newSurfaceList);
            return hasMerged;
        }

        bool mergeTriangles(vector<Triangle *> &triangleList, double thres1, double thres2,
                            vector<Surface *> &newSurfaceList) {
            vector<Surface *> surfaceList;
            for (Triangle* tri : triangleList)
                surfaceList.push_back(new Surface(tri));
            SurfaceMerger sm(thres1, thres2);
            bool hasMerged = sm.mergeSurfaces(surfaceList, newSurfaceList);
            for(Surface* sf : surfaceList)
                delete sf;
            return hasMerged;
        }


    }
}