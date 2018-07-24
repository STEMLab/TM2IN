//
// Created by dongmin on 18. 7. 23.
//

#ifndef TM2IN_FEATURES_GEOMETRY_H
#define TM2IN_FEATURES_GEOMETRY_H

#include <cstdio>
#include <string>
#include <vector>
#include "detail/cgal_config.h"

class Geometry;
class Vertex;
class Surface;
class PolyhedralSurface;
class TriangleMesh;
class TriangleMeshGraph;
class Triangle;
class HalfEdge;

namespace TM2IN{
    enum class GEOM_TYPE {
        Geometry, PolyhedralSurface, Surface, Triangle, HalfEdge, Vertex
    };
}

class Geometry {
protected:
    double min_coords[3] ={0};
    double max_coords[3] ={0};
    TM2IN::GEOM_TYPE type;
    double area = 0.0;
public:
    Geometry(){
        type = TM2IN::GEOM_TYPE::Geometry;
    }

    double getArea(){
        return this->area;
    }

    void setArea(double _area){
        this->area = _area;
    }

    static bool compareArea(Geometry* i, Geometry* j);
    const double *getMin_coords() const;
    const double *getMax_coords() const;
    virtual void updateMBB(Geometry* gm) = 0;
    virtual std::string asJsonText() = 0;
};

#endif //TM2IN_FEATURES_GEOMETRY_H
