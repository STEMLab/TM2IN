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

    bool checkAndSetAdjacent(Triangle *tri);
    bool isOpposite(Triangle* tri);
    int findShareSameHalfEdge(Triangle *pTriangle);

    Vertex* operator[](int idx);
    Vertex* vertex(int idx);

    std::string toJSON(const std::string &indent);
};


#endif // TRIANGLE_H
