//
// Created by dongmin on 18. 8. 13.
//

#include "Polygon.h"


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


        bool Polygon::is_valid_polygon(){
            return true;
        }
    }
}