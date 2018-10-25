//
// Created by dongmin on 18. 8. 9.
//

#ifndef TM2IN_TRIANGULATEDSURFACE_H
#define TM2IN_TRIANGULATEDSURFACE_H

#include "Surface.h"

namespace TM2IN {
    namespace Wall {
        /**
        * @ingroup geo_features
        * @brief Surface with at least one Triangle
        */
        class TriangulatedSurface : public Surface{

        public:
            std::vector<Triangle*> triangles;

            TriangulatedSurface();

            /**
             * @brief  Constructor with Triangle
             */
            TriangulatedSurface(Triangle& pl);

            /**
             * @brief  Constructor with Triangle
             */
            TriangulatedSurface(Triangle* pl) : TriangulatedSurface(*pl) {}

            /**
             * @brief  Constructor with the other Surface
             */
            TriangulatedSurface(TriangulatedSurface *pSurface);

            /**
             * @brief Validates with strict restriction
             */
            bool strict_validation();

            /**
             * @brief Validates with strict restriction
             */
            bool easy_validation();

            bool updateNormal() override;

            std::vector<Wall::TriangulatedSurface *> getNeighborList();
        };
    }
}


#endif //TM2IN_TRIANGULATEDSURFACE_H
