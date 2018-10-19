//
// Created by dongmin on 18. 8. 13.
//

#include "Polygon.h"
#include "features/HalfEdge.h"

namespace TM2IN {
    namespace Wall {

        bool Polygon::updateNormal() {
            return false;
        }

        Polygon::Polygon(Surface * pSurface) {
            this->exteriorBoundary = pSurface->getExteriorBoundary();
            this->normal = pSurface->normal;
            // this->mbb = new MinimumBoundingBox(pSurface->getMBB());
            this->mbb = pSurface->getMBB();
            this->area = pSurface->getArea();
            this->geom_id = pSurface->geom_id;
            this->type = TM2IN::IND_TYPE::Polygon;
            if (!is_valid_polygon()){
                throw std::runtime_error("This Surface cannot be polygon..");
            }
        }


        /**
         * @todo impl
         * @return
         */
        bool Polygon::is_valid_polygon(){
            return true;
        }

        Polygon::Polygon(vector<Vertex *> vt_list) {
            for (ull index = 0 ; index < vt_list.size() - 1 ; index++ ){
                exteriorBoundary.push_back(new HalfEdge(vt_list[index], vt_list[index + 1], this));
            }
            exteriorBoundary.push_back(new HalfEdge(vt_list[vt_list.size() - 1], vt_list[0], this));
            this->area = 0.0;
            this->updateMBB();
        }
    }
}