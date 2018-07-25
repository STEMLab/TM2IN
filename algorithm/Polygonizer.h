//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_POLYGONIZER_H
#define TM2IN_POLYGONIZER_H

#include <vector>
#include "features/PolyhedralSurface.h"

using namespace std;

namespace TM2IN {
    namespace algorithm {
        class Polygonizer {
        public:
            virtual void run(PolyhedralSurface* space) = 0;
        };

        class TrianglePolygonizer : public Polygonizer{
        public:
            void run(PolyhedralSurface* space);
        };

        class DividedPolygonizer : public Polygonizer {
        public:
            void run(PolyhedralSurface* space);
        };

        class PCAPolygonizer : public Polygonizer {
        public:
            void run(PolyhedralSurface* space);
        private:
            void scale_up(vector<Vertex *> &vertices, double scale);
            // SFCGAL::Polygon make_sf_polygon(vector<Vertex *> vertices);
        };
    }
}


#endif //TM2IN_POLYGONIZER_H
