//
// Created by dongmin on 18. 7. 23.
//

#include "features/IndoorComponent.h"

namespace TM2IN{
    MinimumBoundingBox *IndoorComponent::getMBB() {
        return this->mbb;
    }

    void IndoorComponent::setMBB(MinimumBoundingBox *_mbb) {
        this->mbb = _mbb;
    }

    IndoorComponent::IndoorComponent() {
        type = TM2IN::IND_TYPE::IndoorComponent;
        this->mbb = new MinimumBoundingBox();
    }

    void IndoorComponent::updateMBB() {
        mbb = new MinimumBoundingBox();
    }

    std::string IndoorComponent::asJsonText() {
        return std::__cxx11::string();
    }

    void IndoorComponent::mergeMBB(IndoorComponent *gm) {
        CGAL::Bbox_3 bbox = gm->getMBB()->CGAL_bbox3();
        mbb->merge(bbox);
    }
}

