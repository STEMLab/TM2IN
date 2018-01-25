//
// Created by dongmin on 18. 1. 18.
//

#include "SurfaceComputation.h"
#include "VertexComputation.h"
#include "compute/VertexListComputation.h"
#include "HalfEdgeComputation.h"


void SurfaceComputation::flatten(Surface *&sf) {
    Plane_3 plane = getPlane3(sf);

    vector<Vertex*> newVertices;
    for (ull index = 0 ; index < sf->sizeOfVertices() ; index++ )
    {
        Point_3 point = VertexComputation::getCGALPoint(sf->vertex(index));
        Point_3 projected = plane.projection(point);

        Vertex* v = new Vertex(projected.x(), projected.y(), projected.z());
        newVertices.push_back(v);
    }

    assert(newVertices.size() == sf->sizeOfVertices());
    sf->setVertices(newVertices);
    sf->planeRef = plane;
}

Vertex* SurfaceComputation::getCenterPoint(Surface *pSurface) {
    Vertex* center = VertexListComputation::getCenter(pSurface->getVerticesList());
    return center;
}

Plane_3 SurfaceComputation::getPlane3(Surface *&pSurface) {
    return VertexListComputation::getPlane3WithPCA(pSurface->getVerticesList());
}

int SurfaceComputation::intersectionCount = 0;

bool SurfaceComputation::resolveIntersection(Surface *&pSurface) {
    bool hasIntersection = true;
    while (hasIntersection){
        hasIntersection = false;
        vector<HalfEdge* > edges = pSurface->getboundaryEdgesList();
        vector<Segment_3> segments;
        for (int i = 0 ; i < edges.size() ; i++){
            segments.push_back(HalfEdgeComputation::getCGALSegment_3(edges[i]));
            delete edges[i];
        }

        for (int i = 1 ; i < segments.size() - 1 ; i++){
            for (int j = 0 ; j < i ; j++){
                if (i-j == 1) continue;
                if (CGAL::do_intersect(segments[i], segments[j])){
//                    cout << "\n\nself intersect !!"<< segments.size() << endl;
//                    cout << i << ", " << j << endl;
//                    cout << segments[i] << endl;
//                    cout << segments[j] << endl;
                    CGAL::cpp11::result_of<Intersect_3(Segment_3, Segment_3)>::type
                            result = CGAL::intersection(segments[i], segments[j]);
                    if (result){
                        if (const Point_3* p = boost::get<Point_3>(&*result)){
                            std::cout << *p << endl;
                        }
                    }
                    intersectionCount++;
                    hasIntersection = true;

                    // remove Vertex.
                    pSurface->removeVertexByIndex(i + 1);
                    i = segments.size();
                    break;
                }
            }
        }
        //if (hasIntersection) return true;
    }
    return hasIntersection;
}

void SurfaceComputation::triangulate(Surface *&pSurface) {
    std::vector<Vertex*> vertexList = pSurface->getVerticesList();
    vector<pair<D_Point, unsigned >> points;
    for (int i = 0 ; i < vertexList.size() ; i++){
        Plane_3 pl = pSurface->planeRef;
        Point_2 point2d = pl.to_2d(VertexComputation::getCGALPoint(vertexList[i]));
        points.push_back(make_pair(D_Point(point2d.x(), point2d.y()), i));
    }

    Delaunay T(points.begin(),points.end());

    CGAL_assertion(T.number_of_vertices() == vertexList.size());

    pSurface->triangles.clear();
    for(Delaunay::Finite_faces_iterator fit = T.finite_faces_begin();
        fit != T.finite_faces_end(); ++fit)
    {
        vector<Vertex*> temp;
        Delaunay::Face_handle face = fit;

        for (int i = 0 ; i < 3 ; i++){
            temp.push_back(vertexList[face->vertex(i)->info()]);
        }

        pSurface->triangles.push_back(new Triangle(temp));
    }
}
