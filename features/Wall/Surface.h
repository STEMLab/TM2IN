//
// Created by dongmin on 18. 8. 7.
//

#ifndef TM2IN_WALL_SURFACE_H
#define TM2IN_WALL_SURFACE_H

#include <features/IndoorComponent.h>
#include <util.h>

using namespace std;

namespace TM2IN{
    namespace Wall{
        /**
        * @ingroup geo_features
        * @brief Mesh consists of only TM2IN::Triangle
        */
        class Surface : public IndoorComponent{
        private:
        protected:
            /**
             * @brief Empty Surface constructor
             */
            Surface(){}

            std::vector<HalfEdge* > innerEdges;
            std::vector<HalfEdge* > exteriorBoundary;

            std::vector<Triangle *> triangulation;
        public:
            Vector_3 normal = CGAL::NULL_VECTOR;

            /**
             * @brief Gets the size of Vertex list
             */
            ull getVerticesSize(){ return exteriorBoundary.size(); }

            /**
             * @brief Set specific Vertex with index
             */
            virtual void setVertex(int index, Vertex* vt);
            /**
             * @brief Read Vertex with index
             */
            virtual Vertex* vertex(int index);

            /**
             * @brief Insert Vertex with index
             * @todo implements
             */
            void insertVertex(int index, Vertex* vt);

            /**
             * @brief Set normal vector
             */
            void setNormal(Vector_3 _normal){ this->normal = _normal; }

            /**
             * @brief  Gets normal vector
             */
            Vector_3 getNormal();

            /**
             * @brief ostream operator
             */
            friend std::ostream& operator<<(std::ostream& ou, Surface* pSurface);

            /**
             * @brief Gets a list of all Vertex
             */
            std::vector<Vertex *> getVerticesList();

            /**
             * @brief Sets Vertex list
             */
            void setVertexList(std::vector<Vertex *> vertices);

            /**
             * @brief Gets a exterior boundary
             */
            std::vector<HalfEdge *> getExteriorBoundary();

            /**
             * @brief Sets a exterior boundary with a vector of HalfEdge
             */
            void setExteriorBoundary(std::vector<HalfEdge *> edges);

            /**
             * @brief Returns the i-th edge.
             */
            HalfEdge *exterior_boundary_edge(int i);

            /**
             * @brief Returns the index of pEdge
             */
            int index_of_exterior_boundary_edge(HalfEdge *pEdge);

            /**
             * @brief Checks Surface has duplicate vertex
             */
            bool has_duplicate_vertex();

            /**
             * @brief Checks Surface is simple
             */
            bool is_simple();

            /**
             * @brief Gets the number of edges between start index and end index.
             */
            int getSegmentsNumber(ll start_index, ll end_index);


            /**
             * @brief Compares with Surface length
             */
            static bool compareLength(Surface* i, Surface* j);

            /**
             * @brief
             */
            bool isOpposite(Surface* sf);

            /**
             * @brief
             */
            void updateMBB();

            /**
             * @brief Triangulate Surface.
             */
            std::vector<Wall::Triangle*> getTriangulation();

            /**
             * @brief Updates normal vector
             */
            virtual bool updateNormal() = 0;
            /**
             * @brief
             */
            std::string asJsonText();
        };
    }
}

#endif //TM2IN_WALL_SURFACE_H
