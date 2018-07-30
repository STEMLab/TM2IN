//
// Created by dongmin on 18. 7. 24.
//

#ifndef TM2IN_MINIMUMBOUNDINGBOX_H
#define TM2IN_MINIMUMBOUNDINGBOX_H


#include <CGAL/Bbox_3.h>

namespace TM2IN {
    class MinimumBoundingBox {
    private:
        double minx, miny, minz;
        double maxx, maxy, maxz;

        CGAL::Bbox_3 cgal_bbox3;
    public:
        MinimumBoundingBox();
        MinimumBoundingBox(double x1, double y1, double z1, double x2, double y2, double z2);

        double operator[](int index);
        double max(int i);
        double min(int i);

        void set_min_coords(double pDouble[3]);
        void set_max_coords(double pDouble[3]);

        void set(CGAL::Bbox_3 &bbox3);
        void merge(CGAL::Bbox_3& _bbox3);
        CGAL::Bbox_3 CGAL_bbox3();

        MinimumBoundingBox(MinimumBoundingBox *pBox);
    };
}


#endif //TM2IN_MINIMUMBOUNDINGBOX_H
