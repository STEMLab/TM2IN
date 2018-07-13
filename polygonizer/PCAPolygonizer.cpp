//
// Created by dongmin on 18. 7. 10.
//

#include <compute/SurfacesListComputation.h>
#include <features/Vertex.h>
#include <compute/Connect_halfedges.h>
#include <cgal/Features_to_CGAL_object.h>
#include <cgal/SurfaceIntersection.h>
#include "PCAPolygonizer.h"

#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiLineString.h>

void PCAPolygonizer::make(Solid *space) {
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

        Plane_3 plane = CGAL_User::make_PCA_plane(triangle_vertices, sf->normal);

        vector<Vertex*> newVertices;
        for (ull index = 0 ; index < sf->getVerticesSize() ; index++ )
        {
            Point_3 point = CGAL_User::getCGALPoint(sf->vertex(index));
            Point_3 projected = plane.projection(point);
            Vertex* v = new Vertex(projected.x(), projected.y(), projected.z());
            newVertices.push_back(v);
        }
        newVertices.push_back(newVertices[0]);
        scale_up(newVertices, 1.2);
        polygons.push_back(make_sf_polygon(newVertices));

        assert(newVertices.size() == sf->getVerticesSize());
        sf->setVertexList(newVertices);
        sf->normal = plane.orthogonal_vector();

        assert(!SurfaceIntersection::checkSelfIntersection(sf));

    }


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


Plane_3 CGAL_User::make_PCA_plane(vector<Vertex *> vertices, Vector_3 normal) {
    vector<Point_3> cgal_points;
    for (int i = 0 ; i < vertices.size() ; i++){
        cgal_points.push_back(CGAL_User::getCGALPoint(vertices[i]));
    }
    Plane_3 plane;
    linear_least_squares_fitting_3(cgal_points.begin(), cgal_points.end(), plane, CGAL::Dimension_tag<0>());

    Vector_3 planeVector = plane.orthogonal_vector();
    CGAL_assertion(normal != CGAL::NULL_VECTOR);
    if (CGALCalculation::getAngle(planeVector, normal) > 90){
        return plane.opposite();
    }
    else
        return plane;
}

