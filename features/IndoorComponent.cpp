//
// Created by dongmin on 18. 7. 23.
//

#include "features/IndoorComponent.h"

namespace TM2IN{
    CGAL::Bbox_3 IndoorComponent::getMBB() {
        return this->mbb;
    }

    void IndoorComponent::setMBB(CGAL::Bbox_3 _mbb) {
        this->mbb = _mbb;
    }

    IndoorComponent::IndoorComponent() {
        type = TM2IN::IND_TYPE::IndoorComponent;
        this->mbb = CGAL::Bbox_3();
    }

    void IndoorComponent::updateMBB() {
        mbb = CGAL::Bbox_3();
    }

    std::string IndoorComponent::asJsonText() {
        return std::__cxx11::string();
    }

    void IndoorComponent::mergeMBB(IndoorComponent *gm) {
        CGAL::Bbox_3 bbox = gm->getMBB();
        this->mbb += bbox;
    }
}

