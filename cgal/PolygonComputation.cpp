//
// Created by dongmin on 18. 1. 29.
//

#include "PolygonComputation.h"


Polygon_2 PolygonComputation::makePolygon(vector<Point_2> surface2D) {
    Polygon_2 polygon;
    for (int i = 0 ; i < surface2D.size() ; i++){
        Traits::Point_2 pt(surface2D[i].x(), surface2D[i].y());
        polygon.push_back(pt);
    }
    return polygon;
}

std::vector<Polygon_2> PolygonComputation::convexPartition(Polygon_2 polygon) {
    vector<Polygon_2> partitionPolygon;
    CGAL::approx_convex_partition_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(partitionPolygon));
    return partitionPolygon;
}
