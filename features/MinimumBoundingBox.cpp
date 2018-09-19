//
// Created by dongmin on 18. 7. 24.
//

#include <stdexcept>
#include <detail/cgal/type_conversion.h>
#include "MinimumBoundingBox.h"

namespace TM2IN {
    MinimumBoundingBox::MinimumBoundingBox() {
        this->cgal_bbox3 = CGAL::Bbox_3();
    }

    MinimumBoundingBox::MinimumBoundingBox(double x1, double y1, double z1, double x2, double y2, double z2) {
        this->cgal_bbox3 = CGAL::Bbox_3(x1,y1,z1,x2,y2,z2);
    }

    MinimumBoundingBox::MinimumBoundingBox(MinimumBoundingBox *pBox) {
        this->cgal_bbox3 = pBox->cgal_bbox3;
    }

    double MinimumBoundingBox::operator[](int index) {
        if (index == 0)
            return this->cgal_bbox3.xmin();
        else if (index == 1)
            return this->cgal_bbox3.ymin();
        else if (index == 2)
            return this->cgal_bbox3.zmin();
        else if (index == 3)
            return this->cgal_bbox3.xmax();
        else if (index == 4)
            return this->cgal_bbox3.ymax();
        else if (index == 5)
            return this->cgal_bbox3.zmax();
        throw std::runtime_error("MinimumBoundingBox::operator[] : out of range");
    }

    double MinimumBoundingBox::max(int index) {
        if (index == 0)
            return this->cgal_bbox3.xmax();
        else if (index == 1)
            return this->cgal_bbox3.ymax();
        else if (index == 2)
            return this->cgal_bbox3.zmax();
        throw std::runtime_error("MinimumBoundingBox::max(int index) : out of range");
    }

    double MinimumBoundingBox::min(int index) {
        if (index == 0)
            return this->cgal_bbox3.xmin();
        else if (index == 1)
            return this->cgal_bbox3.ymin();
        else if (index == 2)
            return this->cgal_bbox3.zmin();
        throw std::runtime_error("MinimumBoundingBox::max(int index) : out of range");
    }

    void MinimumBoundingBox::set_coords(double pDouble[6]) {
        this->cgal_bbox3 = CGAL::Bbox_3(pDouble[0], pDouble[1], pDouble[2], pDouble[3], pDouble[4], pDouble[5]);
    }

    void MinimumBoundingBox::set(CGAL::Bbox_3 &bbox3) {
        this->cgal_bbox3 = bbox3;
    }

    CGAL::Bbox_3 MinimumBoundingBox::CGAL_bbox3() {
        if (this->cgal_bbox3 == CGAL::Bbox_3()) // need update
            this->cgal_bbox3 = TM2IN::detail::cgal::to_CGAL_bbox3(*this);
        return this->cgal_bbox3;
    }

    void MinimumBoundingBox::merge(CGAL::Bbox_3& _bbox3) {
        if (this->cgal_bbox3 == CGAL::Bbox_3()) // need update
            this->cgal_bbox3 = TM2IN::detail::cgal::to_CGAL_bbox3(*this);
        this->cgal_bbox3 += _bbox3;
        set(this->cgal_bbox3);
    }


}