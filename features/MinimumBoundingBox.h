//
// Created by dongmin on 18. 7. 24.
//

#ifndef TM2IN_MINIMUMBOUNDINGBOX_H
#define TM2IN_MINIMUMBOUNDINGBOX_H


#include <CGAL/Bbox_3.h>

namespace TM2IN {
    /**
     * @ingroup geo_features
     */
    class MinimumBoundingBox {
    private:
        CGAL::Bbox_3 cgal_bbox3;
    public:
        /**
         * @brief Empty MBB constructor
         */
        MinimumBoundingBox();
        /**
         * @brief MinimumBoundingBox constructor.
         */
        MinimumBoundingBox(double x1, double y1, double z1, double x2, double y2, double z2);
        /**
         * @brief MinimumBoundingBox constructor
         */
        MinimumBoundingBox(MinimumBoundingBox *pBox);

        /**
         * @brief Operator []. 0 : x, 1 : y, 2 : z
         */
        double operator[](int index);

        /**
         * @brief Return maximum value. 0 : x, 1 : y, 2 : z
         */
        double max(int i);
        /**
         * @brief Return minimum value. 0 : x, 1 : y, 2 : z
         */
        double min(int i);

        /**
         * @brief Sets coordinates
         */
        void set_coords(double pDouble[6]);

        /**
         * @brief Sets MBB by CGAL::Bbox_3
         */
        void set(CGAL::Bbox_3 &bbox3);

        /**
         * @brief Merges with CGAL::Bbox_3
         */
        void merge(CGAL::Bbox_3& _bbox3);

        /**
         * @brief Converts to CGAL:Bbox_3
         */
        CGAL::Bbox_3 CGAL_bbox3();


    };
}


#endif //TM2IN_MINIMUMBOUNDINGBOX_H
