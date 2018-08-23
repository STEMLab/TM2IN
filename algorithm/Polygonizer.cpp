//
// Created by dongmin on 18. 7. 19.
//

#include "Polygonizer.h"

#include <detail/features/halfedge_string.h>

#include "features/Wall/Triangle.h"
#include "features/Wall/TriangulatedSurface.h"
#include "features/Wall/Polygon.h"
#include "features/Vertex.h"
#include "detail/cgal/plane.h"
#include "algorithm/merge_surfaces.h"
#include "features/RoomBoundary/PolygonMesh.h"

namespace TM2IN{
    namespace algorithm{
        TM2IN::RoomBoundary::PolygonMesh* TrianglePolygonizer::run(TM2IN::RoomBoundary::TSM *space) {
            vector<TM2IN::Wall::Triangle*> triangleList;
            for (unsigned int sfID = 0 ; sfID < space->surfaces.size(); sfID++) {
                TM2IN::Wall::TriangulatedSurface* pSurface = space->surfaces[sfID];
                vector<TM2IN::Wall::Triangle*> triangulation = pSurface->getTriangulation();
                triangleList.insert(triangleList.end(),triangulation.begin(),triangulation.end());
            }

            vector<TM2IN::Wall::Polygon*> newSurfaceList;
            for (int i = 0 ; i < triangleList.size() ; i++){
                newSurfaceList.push_back(new Polygon(triangleList[i]));
            }

            TM2IN::RoomBoundary::PolygonMesh* pm = new TM2IN::RoomBoundary::PolygonMesh(newSurfaceList);
            return pm;
            //space->surfaces = newSurfaceList;
        }

        TM2IN::RoomBoundary::PolygonMesh* DividedPolygonizer::run(TM2IN::RoomBoundary::TSM *space) {
            double thres1 = 1.0;
            double thres2 = 45.0;
            vector<TM2IN::Wall::Surface*> newSurfaceList;
            for (int i = 0 ; i < space->surfaces.size() ; i++){
                TM2IN::Wall::TriangulatedSurface* sf = space->surfaces[i];
                vector<TM2IN::Wall::TriangulatedSurface*> newSurfacesInSurface;
                vector<TM2IN::Wall::Triangle*> triangulation = sf->getTriangulation();

                TM2IN::detail::HalfEdgeString::connectOppositeHalfEdges(triangulation);
                TM2IN::algorithm::mergeTriangles(triangulation, thres1, thres2, newSurfacesInSurface);
                newSurfaceList.insert(newSurfaceList.end(), newSurfacesInSurface.begin(), newSurfacesInSurface.end());
            }

            vector<TM2IN::Wall::Polygon*> newPolygonList;
            for (int i = 0 ; i < newSurfaceList.size() ; i++){
                newPolygonList.push_back(new Polygon(newSurfaceList[i]));
            }

            TM2IN::RoomBoundary::PolygonMesh* pm = new TM2IN::RoomBoundary::PolygonMesh(newPolygonList);
            return pm;
        }


        TM2IN::RoomBoundary::PolygonMesh* PCAPolygonizer::run(TM2IN::RoomBoundary::TSM *space) {
            throw std::runtime_error("PCAPolygonizer run not yet");
            /*
            vector<Surface*> newSurfacesList;
            vector<SFCGAL::Polygon> polygons;
            for (int i = 0 ; i < space->surfacesList.size() ; i++){
                Surface* sf = space->surfacesList[i];

                vector<Vertex*> triangle_vertices;
                for (Triangle* tri : sf->triangles){
                    vector<Vertex*> i_vertices = tri->getVerticesList();
                    for (Vertex* v : i_vertices){
                        triangle_vertices.emplace_back(v);
                    }
                }

                Plane_3 plane = TM2IN::detail::cgal::make_PCA_plane(triangle_vertices, sf->normal);

                vector<Vertex*> newVertices;
                for (ull index = 0 ; index < sf->getVerticesSize() ; index++ )
                {
                    Point_3 point = sf->vertex(index)->CGAL_point();
                    Point_3 projected = plane.projection(point);
                    Vertex* v = new Vertex(projected.x(), projected.y(), projected.z());
                    newVertices.push_back(v);
                }
                assert(newVertices.size() == sf->getVerticesSize());
                sf->setVertexList(newVertices);
                sf->normal = plane.orthogonal_vector();

                newVertices.push_back(newVertices[0]);
                scale_up(newVertices, 1.2);
                polygons.push_back(make_sf_polygon(newVertices));

                // assert(!SurfaceIntersection::check_surface_is_simple(sf));
            }
            */

            /*
            // get intersections
            vector<SFCGAL::LineString*> intersection_lines;
            for (int i = 0 ; i < polygons.size() - 1 ; i++){
                for (int j = i + 1 ; j < polygons.size() ; j++){
                    std::auto_ptr<SFCGAL::Geometry> pt = SFCGAL::algorithm::intersection3D(polygons[i], polygons[j]);
                    if (pt.get()->geometryTypeId() == 2){
                        SFCGAL::LineString* line = dynamic_cast<SFCGAL::LineString*>(pt.release());
                        intersection_lines.push_back(line);
                    }
                    else if(pt.get()->geometryTypeId() != 7){
                        SFCGAL::MultiLineString* lines = dynamic_cast<SFCGAL::MultiLineString*>(pt.release());
                        for (int line_i = 0 ; line_i < lines->numGeometries() ; line_i++){
                            intersection_lines.push_back(&lines->lineStringN(line_i));
                        }
                    }
                    else {
                        cout << pt.get()->asText(5) << endl;
                        cout << pt.get()->geometryTypeId() << endl;
                    }

                }
            }

            // connect intersections
            for (int i = 0 ; i < intersection_lines.size() ; i++){

                cout << intersection_lines[i]->asText(5) << endl;
            }

            //
        */
        }


        void PCAPolygonizer::scale_up(vector<Vertex *> &vertices, double scale){
            // translate to 0, 0
            double center[3] ={0};
            for (ull index = 0 ; index < vertices.size() ; index++){
                center[0] += vertices[index]->x();
                center[1] += vertices[index]->y();
                center[2] += vertices[index]->z();
            }

            center[0] /= (double)vertices.size();
            center[1] /= (double)vertices.size();
            center[2] /= (double)vertices.size();

            double diff[3] = {-center[0], -center[1], -center[2]};

            for (ull index = 0 ; index < vertices.size() ; index++){
                vertices[index]->translate(diff);
                vector<double> dest_vec = {vertices[index]->x() * scale, vertices[index]->y() * scale, vertices[index]->z() * scale};
                vertices[index]->translateTo(dest_vec);
                vertices[index]->translate(center);
            }
        }
        /*
        SFCGAL::Polygon PCAPolygonizer::make_sf_polygon(vector<Vertex *> vertices) {
            vector<SFCGAL::Point> sf_points;
            for (ull index = 0 ; index < vertices.size() ; index++) {
                SFCGAL::Coordinate coord(vertices[index]->x(), vertices[index]->y(), vertices[index]->z());
                sf_points.push_back(SFCGAL::Point(coord));
            }
            SFCGAL::LineString sf_ls(sf_points);
            SFCGAL::Polygon sf_pl(sf_ls);
            return sf_pl;
        }
        */
    }
}