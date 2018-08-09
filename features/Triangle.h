#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "features/IndoorComponent.h"
#include "features/Surface.h"

namespace TM2IN {
    /**
     * @ingroup geo_features
     */
    class Triangle : public Surface{
    public:
        /**
         * @brief Empty Triangle constructor
         */
        Triangle() {}

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
         * @brief Checks the other triangle has opposite HalfEdge
         */
        bool checkNeighbor(Triangle *&tri);

        /**
         * @brief
         */
        bool checkOppositeEdge(Triangle *&tri);

        /**
         * @brief Sets neighbor relationship if Triangle has opposite HalfEdge
         *
         */
        bool setNeighbor(Triangle *&tri);

        /**
         *  @brief Checks the other Triangle has three opposite HalfEdge
         */
        bool isOpposite(Triangle *tri);

        /**
         * @brief
         */
        int findShareSameHalfEdge(Triangle *pTriangle);

        /**
         * @brief Operator [] with index
         */
        Vertex *operator[](int idx);

        /**
         * @brief Access Vertex with index
         */
        Vertex *vertex(int idx);

        /**
         * @brief Converts to Kernel::Triangle_3
         */
        Kernel::Triangle_3 CGAL_triangle();

        /**
         * @brief Gets a list of HalfEdge.
         */
        std::vector<HalfEdge *> getExteriorBoundary();
    };
}

#endif // TRIANGLE_H
