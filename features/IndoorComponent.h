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
    namespace Wall{
        class Surface;
        class Triangle;
        class TriangulatedSurface;
        class Polygon;
    }
    class Vertex;
    class HalfEdge;

    enum class IND_TYPE {
        IndoorComponent, Room, RoomBoundary, TriangulatedSurface, Triangle, Polygon, HalfEdge, Vertex
    };
/**
     * @ingroup geo_features
     */
    class IndoorComponent {
    protected:
        // MinimumBoundingBox* mbb;
        CGAL::Bbox_3 mbb;
        TM2IN::IND_TYPE type;
        double area = 0.0;
    public:
        std::string geom_id;

        IndoorComponent();

        /**
         * @brief Returns area
         */
        double getArea(){
            return this->area;
        }
        /**
         * @brief Sets area
         */
        void setArea(double _area){
            this->area = _area;
        }
        /**
         * @brief Returns minimum bounding box
         */
        CGAL::Bbox_3 getMBB();

        /**
         * @brief Sets minimum bounding box
         */
        void setMBB(CGAL::Bbox_3 _mbb) ;

        /**
         * @brief Updates MinimumBoundingBox
         */
        virtual void updateMBB();

        /**
         *
         *  @brief Merges with the other IndoorComponent.
         */
        virtual void mergeMBB(IndoorComponent* gm);

        /**
         * @brief Generates json string.
         */
        virtual std::string asJsonText();
    };

}

#endif //TM2IN_FEATURES_GEOMETRY_H
