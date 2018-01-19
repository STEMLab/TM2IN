//
// Created by dongmin on 18. 1. 18.
//

#include "SurfaceComputation.h"
#include "VertexComputation.h"
#include "compute/VertexListComputation.h"

void SurfaceComputation::flatten(Surface *&sf) {
    Point_3 center = VertexComputation::getCGALPoint(getCenterPoint(sf));
    CGAL_assertion(sf->av_normal != CGAL::NULL_VECTOR);
    Plane_3 plane = Plane_3(center, sf->av_normal);

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
}

Vertex* SurfaceComputation::getCenterPoint(Surface *pSurface) {
    Vertex* center = VertexListComputation::getCenter(pSurface->getVerticesList());
    return center;
}
