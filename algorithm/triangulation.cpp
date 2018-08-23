//
// Created by dongmin on 18. 7. 20.
//

#include "triangulation.h"

#include "features/Wall/Surface.h"
#include "features/Wall/Triangle.h"
#include "detail/algorithm/triangulate_surface.h"

using namespace std;
using namespace TM2IN;

namespace TM2IN {
    namespace algorithm {
        int triangulate(TM2IN::Wall::Surface *pSurface, std::vector<TM2IN::Wall::Triangle*>& result) {
            result.clear();

            std::vector<Vertex*> vertexList = pSurface->getVerticesList();
            pSurface->updateNormal();

            if (vertexList.size() == 3) {
                TM2IN::Wall::Triangle* newTriangle = new TM2IN::Wall::Triangle(vertexList);
                newTriangle->geom_id = pSurface->geom_id + "_0";
                result.push_back(newTriangle);
                return 0;
            }
            else {
                int return_value = TM2IN::detail::algorithm::triangulate_surface(pSurface, result);
                Vector_3 vt_normal(0,0,0);
                for (Wall::Triangle* tri : result){
                    vt_normal = vt_normal + tri->normal;
                }
                pSurface->normal = vt_normal;
                return return_value;
            }
        }
    }
}