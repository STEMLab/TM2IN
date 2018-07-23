#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "features/Vertex.h"
#include "features/Surface.h"
#include "predefine.h"
#include "cgal/CGALCalculation.h"

class Triangle : public Surface {
public:
    Triangle(){}

    Triangle(Vertex* pa, Vertex *pb, Vertex* pc);

    Triangle(std::vector<Vertex*>& vertices) : Triangle(vertices[0], vertices[1], vertices[2]) {}

    bool checkNeighbor(Triangle*& tri);
    bool checkOppositeEdge(Triangle*& tri);
    bool setNeighbor(Triangle*& tri);
    bool isOpposite(Triangle* tri);
    int findShareSameHalfEdge(Triangle *pTriangle);

    Vertex* operator[](int idx);
    Vertex* vertex(int idx);


    Kernel::Triangle_3 CGAL_triangle();
};


#endif // TRIANGLE_H
