//
// Created by dongmin on 18. 8. 13.
//

#ifndef TM2IN_POLYGON_H
#define TM2IN_POLYGON_H

#include "Surface.h"

namespace TM2IN {
    namespace Wall {
        class Polygon : public Surface{
        public:
            Polygon(Surface* );

            /**
             * @brief Checks this polygon is valid.
             * @todo imp
             */
            bool is_valid_polygon();

            bool updateNormal() override;
        };
    }
}


#endif //TM2IN_POLYGON_H
