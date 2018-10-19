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
            cout << "===== Run Triangle Polygonizer =====" << endl;
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
            cout << "===== Run Divided Polygonizer =====" << endl;
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
            vector<TM2IN::Wall::Polygon*> newPolygonList;
            for (unsigned int sfID = 0 ; sfID < space->surfaces.size(); sfID++) {
                Surface* sf = space->surfaces[sfID];
                vector<Vertex*> vertices = sf->getVerticesList();
                Plane_3 plane = TM2IN::detail::cgal::make_PCA_plane(vertices, sf->normal);

                vector<Vertex*> newVertices;
                for (ull index = 0 ; index < vertices.size() ; index++ )
                {
                    Point_3 point = vertices[index]->CGAL_point();
                    Point_3 projected = plane.projection(point);
                    auto vt = new Vertex(projected.x(), projected.y(), projected.z());
                    vt->geom_id = "0";
                    newVertices.push_back(vt);
                }
                assert(newVertices.size() == sf->getVerticesSize());

                TM2IN::Wall::Polygon* new_sf = new TM2IN::Wall::Polygon(newVertices);
                new_sf->normal = plane.orthogonal_vector();
                newPolygonList.push_back(new_sf);
            }
            TM2IN::RoomBoundary::PolygonMesh* pm = new TM2IN::RoomBoundary::PolygonMesh(newPolygonList);
            return pm;
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