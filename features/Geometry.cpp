//
// Created by dongmin on 18. 7. 23.
//

#include "Geometry.h"

const double *Geometry::getMin_coords() const {
    return min_coords;
}

const double *Geometry::getMax_coords() const {
    return max_coords;
}

bool Geometry::compareArea(Geometry* i, Geometry* j) {
    return (i->area > j->area);
}