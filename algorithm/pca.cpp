//
// Created by dongmin on 18. 7. 19.
//
#include "pca.h"

namespace TM2IN {
    namespace algorithm {
        Plane_3 make_PCA_plane(std::vector < Vertex * > vertices, Vector_3 normal) {
            std::vector <Point_3> cgal_points;
            for (int i = 0; i<vertices.size();i++) {
                cgal_points.push_back (vertices[i]->CGAL_point());
            }
            Plane_3 plane;
            linear_least_squares_fitting_3(cgal_points.begin(), cgal_points.end(), plane, CGAL::Dimension_tag<0>());

            Vector_3 planeVector = plane.orthogonal_vector();
            CGAL_assertion(normal!= CGAL::NULL_VECTOR);
            if (CGALCalculation::getAngle(planeVector, normal) > 90) {
                return plane.opposite();
            }
            else
                return plane;
        }
    }
}