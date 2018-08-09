
#ifndef Surface_h
#define Surface_h

#include <vector>
#include <string>
#include <climits>
#include <cassert>

#include "util.h"
#include "features/IndoorComponent.h"
#include "detail/cgal_config.h"

namespace TM2IN{
    /**
     * @ingroup geo_features
     */
    class Surface : public IndoorComponent{
    protected:
        std::vector<HalfEdge* > innerEdges;
        std::vector<HalfEdge* > exteriorBoundary;
    public:
        Vector_3 normal = CGAL::NULL_VECTOR;

        std::vector<Triangle*> triangles;
        std::vector<Triangle *> triangulation;

        /**
         * @brief Empty Surface constructor
         */
        Surface();

        /**
         * @brief  Constructor with Triangle
         */
        Surface(Triangle& pl);

        /**
         * @brief  Constructor with Triangle
         */
        Surface(Triangle* pl) : Surface(*pl) {}

        /**
         * @brief  Constructor with the other Surface
         */
        Surface(Surface* cp);


        /**
         * @brief Gets the size of Vertex list
         */
        ull getVerticesSize(){ return exteriorBoundary.size(); }

        /**
         * @brief Set specific Vertex with index
         * @todo
         */
        void setVertex(int index, Vertex* vt);

        /**
         * @brief Read Vertex with index
         */
        virtual Vertex* vertex(int index);

        /**
         * @brief Insert Vertex with index
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
         * @brief Calculates and gets normal vector by simple way
         */
        Vector_3 getSimpleNormal();


        /**
         * @brief
         */
        virtual std::string asJsonText();

        /**
         * @brief Triangulate Surface.
         */
        std::vector<Triangle*> getTriangulation();


        /**
         * @brief
         */
        virtual bool isOpposite(Surface* sf);


        /**
         * @brief
         */
        void updateMBB();


        /**
         * @brief Gets the number of edges between start index and end index.
         */
        int getSegmentsNumber(ll start_index, ll end_index);


        /**
         * @brief Compares with Surface length
         */
        static bool compareLength(Surface* i, Surface* j);


        /**
         * @brief Updates normal vector
         */
        bool updateNormal();


        /**
         * @brief Validates with strict restriction
         */
        bool strict_validation();

        /**
         * @brief Validates with strict restriction
         */
        bool easy_validation();

        /**
         * @brief Checks Surface has duplicate vertex
         */
        bool has_duplicate_vertex();

        /**
         * @brief Checks Surface is simple
         */
        bool is_simple();

        /**
         * @brief Gets a list of all Vertex
         */
        std::vector<Vertex *> getVerticesList();

        /**
         * @brief Sets Vertex list
         */
        void setVertexList(std::vector<Vertex *> vector);

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
         * @brief ostream operator
         */
        friend std::ostream& operator<<(std::ostream& ou, Surface* pSurface);

    };

}



#endif /* Surface_h */
