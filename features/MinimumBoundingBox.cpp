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
        minx = x1;
        miny = y1;
        minz = z1;
        maxx = x2;
        maxy = y2;
        maxz = z2;
        this->cgal_bbox3 = CGAL::Bbox_3(minx,miny,minz,maxx,maxy,maxz);
    }


    MinimumBoundingBox::MinimumBoundingBox(MinimumBoundingBox *pBox) {
        if (pBox->cgal_bbox3 == CGAL::Bbox_3())
            this->cgal_bbox3 = CGAL::Bbox_3();
        else
            this->update(pBox->cgal_bbox3);
    }

    double MinimumBoundingBox::operator[](int index) {
        if (index == 0)
            return minx;
        else if (index == 1)
            return miny;
        else if (index == 2)
            return minz;
        else if (index == 3)
            return maxx;
        else if (index == 4)
            return maxy;
        else if (index == 5)
            return maxz;
        throw std::runtime_error("MinimumBoundingBox::operator[] : out of range");
    }

    double MinimumBoundingBox::max(int index) {
        if (index == 0)
            return maxx;
        else if (index == 1)
            return maxy;
        else if (index == 2)
            return maxz;
        throw std::runtime_error("MinimumBoundingBox::max(int index) : out of range");
    }

    double MinimumBoundingBox::min(int index) {
        if (index == 0)
            return minx;
        else if (index == 1)
            return miny;
        else if (index == 2)
            return minz;
        throw std::runtime_error("MinimumBoundingBox::max(int index) : out of range");
    }

    void MinimumBoundingBox::set_min_coords(double *pDouble) {
        minx = pDouble[0];
        miny = pDouble[1];
        minz = pDouble[2];
        this->cgal_bbox3 = CGAL::Bbox_3();
    }

    void MinimumBoundingBox::set_max_coords(double *pDouble) {
        maxx = pDouble[0];
        maxy = pDouble[1];
        maxz = pDouble[2];
        this->cgal_bbox3 = CGAL::Bbox_3();
    }

    void MinimumBoundingBox::update(CGAL::Bbox_3& bbox3) {
        maxx = bbox3.xmax();
        maxy = bbox3.ymax();
        maxz = bbox3.zmax();
        minx = bbox3.xmin();
        miny = bbox3.ymin();
        minz = bbox3.zmin();
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
        update(this->cgal_bbox3);
    }


}