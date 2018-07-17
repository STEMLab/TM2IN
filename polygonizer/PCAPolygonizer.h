//
// Created by dongmin on 18. 7. 10.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_PCAPOLYGONIZER_H
#define TRIANGLEMESHTOCLEARSOLID_PCAPOLYGONIZER_H

#include <SFCGAL/Polygon.h>
#include "Polygonizer.h"

using namespace std;

class PCAPolygonizer : public Polygonizer {
public:
    void make(PolyhedralSurface* space);
private:
    void scale_up(vector<Vertex *> &vertices, double scale);

    SFCGAL::Polygon make_sf_polygon(vector<Vertex *> vertices);
};

namespace CGAL_User{
    Plane_3 make_PCA_plane(vector<Vertex *> vertices, Vector_3 normal);
}

#endif //TRIANGLEMESHTOCLEARSOLID_PCAPOLYGONIZER_H
