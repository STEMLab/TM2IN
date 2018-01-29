//
// Created by dongmin on 18. 1. 29.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_POLYGONCOMPUTATION_H
#define TRIANGLEMESHTOCLEARSOLID_POLYGONCOMPUTATION_H

#include "cgal/Types.h"
#include "features/Surface.h"

using namespace std;

class PolygonComputation {
public:
    static Polygon_2 makePolygon(vector<Point_2> surface2D);
    static std::vector<Polygon_2> convexPartition(Polygon_2 polygon);
};


#endif //TRIANGLEMESHTOCLEARSOLID_POLYGONCOMPUTATION_H
