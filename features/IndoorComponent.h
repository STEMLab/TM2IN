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
    namespace RoomBoundary{
        class TriangleMesh;
        class TriangulatedSurfaceMesh;
        class PolygonMesh;
    }
    class Room;

    class Surface;
    class Triangle;
    class Vertex;
    class HalfEdge;
    class Poylgon;

    enum class IND_TYPE {
        IndoorComponent, Room, RoomBoundary, Surface, Triangle, HalfEdge, Vertex
    };

    class IndoorComponent {
    protected:
        MinimumBoundingBox* mbb;
        TM2IN::IND_TYPE type;
        double area = 0.0;
    public:
        std::string geom_id;

        IndoorComponent();

        double getArea(){
            return this->area;
        }

        void setArea(double _area){
            this->area = _area;
        }

        MinimumBoundingBox *getMBB();
        void setMBB(MinimumBoundingBox* _mbb) ;
        virtual void updateMBB();
        virtual void mergeMBB(IndoorComponent* gm);
        virtual std::string asJsonText();
    };

}

#endif //TM2IN_FEATURES_GEOMETRY_H
