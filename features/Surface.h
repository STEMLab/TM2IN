
#ifndef Surface_h
#define Surface_h

#include <vector>
#include <string>
#include <climits>
#include <cassert>

#include "logic/check.h"
#include "util.h"
#include "predefine.h"
#include "cgal/Types.h"

class Surface{
private:
    Plane_3 planeRef;
public:
    std::string sf_id;
    // std::vector<Vertex*> v_list;
    std::vector<HalfEdge* > innerEdges;
    std::vector<HalfEdge* > boundaryEdges;

    Vector_3 normal = CGAL::NULL_VECTOR;

    std::vector<Triangle*> triangles;
    std::vector<Triangle *> triangulation;

    double min_coords[3];
    double max_coords[3];
    double area = 0.0;

    Surface(){}
    Surface(Triangle& pl);
    Surface(Triangle* pl) : Surface(*pl) {}
    Surface(Surface* cp);
    Surface(std::vector<Vertex*>& pVertices);

    ull getVerticesSize(){ return boundaryEdges.size(); }
    void setVertex(int index, Vertex* vt);
    virtual Vertex* vertex(int index);
    void insertVertex(int index, Vertex* vt);

    void setNormal(Vector_3 _normal){ this->normal = _normal; }
    Vector_3 getNormal();
    double getArea();

    std::string asJsonText();
    std::string toJSONWithTriangles();

    bool isInMBB(Vertex* vt);
    bool isOpposite(Surface* sf);

    void setMBB(Surface* pl);
    void updateMBB();
    int getSegmentsNumber(ll start_index, ll end_index);

    //compare vector size
    static bool compareLength(Surface* i, Surface* j);
    static bool compareArea(Surface* i, Surface* j);

    void translate(double diff[]);
    bool checkDuplicate();

    Vector_3 getSimpleNormal();
    bool updateNormal();
    bool isValid();

    Point_3 findLowestPoint();
    Plane_3 getPlaneWithLowest();

    std::vector<Vertex *> getVerticesList();

    void setVertexList(std::vector<Vertex *> vector);

    std::vector<HalfEdge *> getBoundaryEdgesList();
    void setBoundaryEdgesList(std::vector<HalfEdge*> edges);

    void removeVertexByIndex(int id);
    void clearTriangleList();

    void removeVertexByIndex(int startIndex, int endIndex);

    Plane_3 getPlaneRef(){
        return planeRef;
    }
    void setPlaneRef(Plane_3 plane);

    HalfEdge *boundary_edges(int i);

    int indexBoundaryEdge(HalfEdge *pEdge);

    friend std::ostream& operator<<(std::ostream& ou, Surface* pSurface);

};



#endif /* Surface_h */
