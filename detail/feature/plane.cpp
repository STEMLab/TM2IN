//
// Created by dongmin on 18. 7. 19.
//

#include "plane.h"
#include "features/Surface.h"

namespace TM2IN {
    namespace detail {
        namespace feature{
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

            Plane_3 make_simple_plane(Vector_3 pNormal) {
                int type = CGALCalculation::findNormalType6(pNormal);
                Vector_3 normal = CGALCalculation::normal_list6[type];
                Point_3 origin(0,0,0);
                Plane_3 plane3(origin, normal);
                return plane3;
            }

            std::vector<Point_2> project_to_plane(std::vector<Vertex*> vertexList, Plane_3 plane) {
                std::vector<Point_2> pointList;
                for (int i = 0 ; i < vertexList.size(); i++){
                    Point_2 point2d = plane.to_2d(vertexList[i]->CGAL_point());
                    pointList.push_back(point2d);
                }
                return pointList;
            }
        }

    }


}