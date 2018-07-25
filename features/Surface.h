
#ifndef Surface_h
#define Surface_h

#include <vector>
#include <string>
#include <climits>
#include <cassert>

#include "util.h"
#include "features/Geometry.h"
#include "detail/cgal_config.h"

namespace TM2IN{
    class Surface : public Geometry{
    protected:
        std::vector<HalfEdge* > innerEdges;
        std::vector<HalfEdge* > exteriorBoundary;
    public:
        std::string sf_id;

        Vector_3 normal = CGAL::NULL_VECTOR;

        std::vector<Triangle*> triangles;
        std::vector<Triangle *> triangulation;

        Surface();
        Surface(Triangle& pl);
        Surface(Triangle* pl) : Surface(*pl) {}
        Surface(Surface* cp);

        ull getVerticesSize(){ return exteriorBoundary.size(); }
        void setVertex(int index, Vertex* vt);
        virtual Vertex* vertex(int index);
        void insertVertex(int index, Vertex* vt);

        void setNormal(Vector_3 _normal){ this->normal = _normal; }
        Vector_3 getNormal();
        Vector_3 getSimpleNormal();

        virtual std::string asJsonText();
        std::vector<Triangle*> getTriangulation();

        virtual bool isOpposite(Surface* sf);

        void updateMBB(Geometry *gm = NULL);

        int getSegmentsNumber(ll start_index, ll end_index);

        //compare vector size
        static bool compareLength(Surface* i, Surface* j);

        bool updateNormal();
        bool strict_validation();
        bool easy_validation();
        bool has_duplicate_vertex();
        bool is_simple();

        std::vector<Vertex *> getVerticesList();
        void setVertexList(std::vector<Vertex *> vector);

        std::vector<HalfEdge *> getExteriorBoundary();
        void setExteriorBoundary(std::vector<HalfEdge *> edges);

        HalfEdge *exterior_boundary_edge(int i);
        int index_of_exterior_boundary_edge(HalfEdge *pEdge);

        friend std::ostream& operator<<(std::ostream& ou, Surface* pSurface);

    };

}



#endif /* Surface_h */
