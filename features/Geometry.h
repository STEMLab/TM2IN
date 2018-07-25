//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_FEATURES_GEOMETRY_H
#define TM2IN_FEATURES_GEOMETRY_H

#include <cstdio>
#include <string>
#include <vector>
#include "detail/cgal_config.h"
#include "MinimumBoundingBox.h"

namespace TM2IN{
    class PolyhedralSurface;
    class Surface;
    class Triangle;
    class Vertex;
    class HalfEdge;
    class TriangleMesh;

    enum class GEOM_TYPE {
        Geometry, PolyhedralSurface, Surface, Triangle, HalfEdge, Vertex
    };

    class Geometry {
    protected:
        MinimumBoundingBox* mbb;
        TM2IN::GEOM_TYPE type;
        double area = 0.0;
    public:
        Geometry();

        double getArea(){
            return this->area;
        }

        void setArea(double _area){
            this->area = _area;
        }

        MinimumBoundingBox *getMBB();
        void setMBB(MinimumBoundingBox* _mbb) ;
        virtual void updateMBB();
        virtual void mergeMBB(Geometry* gm);
        virtual std::string asJsonText();
    };

}

#endif //TM2IN_FEATURES_GEOMETRY_H
