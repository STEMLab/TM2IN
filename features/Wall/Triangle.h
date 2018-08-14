//
// Created by dongmin on 18. 8. 9.
//

#ifndef TM2IN_WALL_TRIANGLE_H
#define TM2IN_WALL_TRIANGLE_H

#include "features/IndoorComponent.h"
#include "Surface.h"

namespace TM2IN {
    namespace Wall {
        /**
        * @ingroup geo_features
        * @brief Mesh consists of only TM2IN::Triangle
        */
        class Triangle : public Surface{
        public:
            /**
             * @brief Constructor with three Vertex
             *
             */
            Triangle(Vertex *pa, Vertex *pb, Vertex *pc);

            /**
             * @brief Constructor with a Vertex vector
             * @warning The only front three vertices in vector will be used.
             */
            Triangle(std::vector<Vertex *> &vertices) : Triangle(vertices[0], vertices[1], vertices[2]) {}


            /**
             * @brief Sets neighbor relationship if Triangle has opposite HalfEdge
             *
             */
            bool setNeighbor(Triangle *&tri);

            /**
             * @brief Converts to Kernel::Triangle_3
             */
            Kernel::Triangle_3 CGAL_triangle();

            /**
             * @brief Checks the other triangle has opposite HalfEdge
             */
            bool checkNeighbor(Triangle *&tri);
            /**
             *  @brief Checks the other Triangle has three opposite HalfEdge
             */
            bool isOpposite(Triangle *tri);

            bool updateNormal() override;

        };
    }
}


#endif //TM2IN_WALL_TRIANGLE_H
