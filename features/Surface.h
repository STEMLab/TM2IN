
#ifndef Surface_h
#define Surface_h

#include <vector>
#include <string>
#include <climits>
#include <cassert>

#include "features/Triangle.h"
#include "logic/check.hpp"
#include "util.h"
#include "predefine.h"

#include "cgal/Types.h"

class Surface{
public:
    ull sf_id;
    std::vector<Vertex*> v_list;
    std::vector<std::vector<Vertex*> > inner_ring;
    std::vector<HalfEdge* > boundaryEdges;
    Vector_3 normal = CGAL::NULL_VECTOR;
    Plane_3 planeRef;

    std::vector<Triangle*> triangles;
    std::vector<std::vector<int>> globalIndicesOfTriangulation;

    double min_coords[3];
    double max_coords[3];
    double area = 0.0;

    Surface(){
    }
    Surface(Triangle& pl);
    Surface(Triangle* pl) : Surface(*pl) {}
    Surface(Surface* cp);
    Surface(std::vector<Vertex*>& pVertices);

    ull sizeOfVertices(){ return v_list.size(); }

    void setZ(double value);

    std::string toJSONString();
    std::string toJSONWithTriangles();

    bool isInMBB(Vertex* vt);
    bool isAdjacent(Surface* sf);
    bool isOpposite(Surface* sf);

    void setMBB(Triangle* pl);
    void setMBB(Surface* pl);
    void updateMBB();
    int getSegmentsNumber(ll start_index, ll end_index);

    //compare vector size
    static bool compareLength(Surface* i, Surface* j);
    static bool compareArea(Surface* i, Surface* j);

    void translate(double diff[]);
    bool checkDuplicate();

    void removeConsecutiveDuplication();
    void removeStraight(double degree);

    Vector_3 getSimpleNormal();
    bool updateNormal();
    bool isValid();

    Point_3 findLowestPoint();
    Plane_3 getPlaneWithLowest();

    std::vector<Vertex *> getVerticesList();

    Vertex *vertex(ull i);

    void setVertices(std::vector<Vertex *> vector);

    std::vector<HalfEdge *> getboundaryEdgesList();
    void removeVertexByIndex(int id);
    void clearTriangleList();
private:
    std::vector<std::pair<double, double>> project_to_Plane18();
    std::vector<Point_2> get2DPoints(Plane_3 plane);
};



#endif /* Surface_h */
