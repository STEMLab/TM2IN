//
// Created by dongmin on 18. 7. 23.
//

#include "Geometry.h"

namespace TM2IN{
    MinimumBoundingBox *Geometry::getMBB() {
        return this->mbb;
    }

    void Geometry::setMBB(MinimumBoundingBox *_mbb) {
        this->mbb = _mbb;
    }

    Geometry::Geometry() {
        type = TM2IN::GEOM_TYPE::Geometry;
        this->mbb = new MinimumBoundingBox();
    }

    void Geometry::updateMBB() {
        mbb = new MinimumBoundingBox();
    }

    std::string Geometry::asJsonText() {
        return std::__cxx11::string();
    }

    void Geometry::mergeMBB(Geometry *gm) {
        CGAL::Bbox_3 bbox = gm->getMBB()->CGAL_bbox3();
        mbb->merge(bbox);
    }
}

