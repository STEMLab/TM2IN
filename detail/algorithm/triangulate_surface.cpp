//
// Created by dongmin on 18. 7. 20.
//

#include "features/Wall/Surface.h"
#include "features/Wall/Triangle.h"
#include "triangulate_surface.h"

namespace TM2IN {
    namespace detail {
        namespace algorithm {
            int triangulate_surface(TM2IN::Wall::Surface *pSurface, vector<TM2IN::Wall::Triangle *> &result) {
                std::vector<Vertex*> vertexList = pSurface->getVerticesList();
                // convert 3D point to 2D
                Plane_3 planeRef = TM2IN::detail::cgal::make_simple_plane(pSurface->normal);
                vector<Point_2> point2dList = TM2IN::detail::cgal::project_to_plane(pSurface->getVerticesList(), planeRef);

                // partition Surface to convex 2D polygons.
                Polygon_2 polygon = TM2IN::detail::cgal::make_CGAL_polygon(point2dList);
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

                vector<Polygon_2> polygonList = TM2IN::detail::cgal::convexPartition(polygon);

                vector<TM2IN::Wall::Triangle* > triangles;
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
                        TM2IN::Wall::Triangle* new_tri = new TM2IN::Wall::Triangle(localTemp);
                        new_tri->geom_id = pSurface->geom_id + "_" + to_string(triangles.size());
                        triangles.push_back(new_tri);
                    }
                }
                result = triangles;

                return 0;
            }
        }
    }
}