//
// Created by dongmin on 18. 7. 20.
//

#include "triangulate_surface.h"


namespace TM2IN {
    namespace detail {
        namespace algorithm {
            int triangulate_surface(Surface *pSurface, vector<Triangle *> &result) {
                std::vector<Vertex*> vertexList = pSurface->getVerticesList();
                // convert 3D point to 2D
                Plane_3 planeRef = TM2IN::detail::feature::make_simple_plane(pSurface->normal);
                vector<Point_2> point2dList = TM2IN::detail::feature::project_to_plane(pSurface->getVerticesList(), planeRef);

                // partition Surface to convex 2D polygons.
                Polygon_2 polygon = TM2IN::detail::feature::make_CGAL_polygon(point2dList);
                if (!polygon.is_simple())
                {
                    cerr << "polygon is not simple" << endl;
                    cerr << pSurface->asJsonText() << endl;
                    cerr << polygon << endl;
                    return 1;
                }
                if (polygon.orientation() == -1){
                    cerr << polygon << endl;
                    cerr << polygon.orientation() << endl;
                    return 1;
                }

                vector<Polygon_2> polygonList = TM2IN::detail::feature::convexPartition(polygon);

                vector<Triangle* > triangles;
                for (int i = 0 ; i < polygonList.size() ; i++){
                    CGAL_assertion(polygonList[i].is_simple() && polygonList[i].is_convex());

                    Polygon_2 p = polygonList[i];
                    vector<Point_2> points;
                    for (Polygon_2::Vertex_iterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi){
                        Point_2 point2d(vi->x(), vi->y());
                        points.push_back(point2d);
                    }

                    Delaunay T;
                    T.insert(points.begin(),points.end());
                    for(Delaunay::Finite_faces_iterator fit = T.finite_faces_begin();
                        fit != T.finite_faces_end(); ++fit)
                    {
                        vector<Vertex*> localTemp;
                        Delaunay::Face_handle facet = fit;

                        for (int j = 0 ; j < 3 ; j++){
                            Point_2 point2d = facet->vertex(j)->point();
                            int k;
                            for (k = 0 ; k < point2dList.size() ; k++){
                                if (point2d == point2dList[k]) break;
                            }
                            if (k == point2dList.size()){
                                cerr << "new Point" << endl;
                                exit(-1);
                            }
                            localTemp.push_back(vertexList[k]);
                        }
                        Triangle* new_tri = new Triangle(localTemp);
                        new_tri->sf_id = pSurface->sf_id + "_" + to_string(triangles.size());
                        triangles.push_back(new_tri);
                    }
                }
                result = triangles;

                return 0;
            }
        }
    }
}