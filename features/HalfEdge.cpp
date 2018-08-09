//
// Created by dongmin on 18. 1. 12.
//

#include <detail/cgal/type_conversion.h>
#include <iostream>
#include "HalfEdge.h"

namespace TM2IN {
    bool HalfEdge::isSame(HalfEdge *he) {
        if (this->vertices[0] == he->vertices[0]) {
            if (this->vertices[1] == he->vertices[1])
                return true;
        }
        return false;
    }

    Surface *HalfEdge::getParent() const {
        return parent;
    }

    void HalfEdge::setParent(Surface *pSurface) {
        HalfEdge::parent = pSurface;
    }

    HalfEdge *HalfEdge::getOppositeEdge() const {
        return oppositeEdge;
    }

    void HalfEdge::setOppositeEdge(HalfEdge *oppositeEdge) {
        HalfEdge::oppositeEdge = oppositeEdge;
    }

    bool HalfEdge::can_be_opposite_edge(HalfEdge *he) {
        if (this->vertices[0] == he->vertices[1]) {
            if (this->vertices[1] == he->vertices[0])
                return true;
        }
        return false;
    }

    void HalfEdge::setVertex(int i, Vertex *vt) {
        this->vertices[i] = vt;
    }

    Kernel::Segment_3 HalfEdge::to_CGAL_segment() {
        return TM2IN::detail::cgal::to_CGAL_Segment_3(this);
    }

    std::string HalfEdge::asJsonText() {
        std::cerr << "HalfEdge::asJsonText()" << std::endl;
        return std::__cxx11::string();
    }
}
