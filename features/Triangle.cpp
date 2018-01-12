#include "features/Triangle.h"
#include "features/HalfEdge.h"

Triangle::Triangle()
{
    //ctor
}

Triangle::Triangle(Vertex* pa, Vertex *pb, Vertex* pc)
{
    edges.push_back(new HalfEdge(pa, pb, this));
    edges.push_back(new HalfEdge(pb, pc, this));
    edges.push_back(new HalfEdge(pc, pa, this));
}


Vector_3 Triangle::getNormal()
{
    if (this->normal != CGAL::NULL_VECTOR) return this->normal;

    Vertex* va = this->a;
    Vertex* vb = this->b;
    Vertex* vc = this->c;

    this->normal = CGALCalculation::getUnitNormal(va, vb, vc) * AREA_CONST * this->getArea();
    return this->normal;
}

double Triangle::getArea(){
    if (this->area > 0.0) return this->area;
    Vertex* va = this->a;
    Vertex* vb = this->b;
    Vertex* vc = this->c;

    this->area = sqrt(CGALCalculation::getSquaredArea(va, vb, vc));

    return this->area;
}

int Triangle::findShareSameHalfEdge(Triangle *pTriangle){
    int result = -1;
    for (int i = 0 ; i < 3 ; i++){
        for (int j = 0 ; j < 3 ; j++) {
            if (this->edges[i]->isSame(pTriangle->edges[j])){
                if (result == -1) result = i;
                else {
                    exit(-1);
                }
            }
        }
    }
    return result;
}


Vertex* Triangle::operator[](int idx){
    if (idx < 3 && idx >= 0) return (*edges[idx])[0];
    else{
        std::cerr << "Triangle only has three edge" << std::endl;
        exit(-1);
    }
}

bool Triangle::checkAndSetAdjacent(Triangle *tri){
    bool isAdjacent;
    for (int v1 = 0 ; v1 < 3 ; v1++){
        for (int v2 = 0 ; v2 < 3 ;v2++){
            if (this->edges[v1]->isOpposite(tri->edges[v2])){
                assert(!isAdjacent);
                isAdjacent = true;
                this->edges[v1]->setOppositeEdge(tri->edges[v2]);
                tri->edges[v2]->setOppositeEdge(this->edges[v1]);
            }
        }
    }
    return isAdjacent;
}

bool Triangle::isOpposite(Triangle* tri){
    int i = 0;
    for (int j = 0 ; j < 3 ; j++){
        if (this->vertex(i) == tri->vertex(j)){
            while (this->vertex(i) == tri->vertex(j)){
                i++; j--;
                if (j < 0) j = 2;
                if (i == 3) break;
                if (this->vertex(i) != tri->vertex(j)){
                    break;
                }

            }
        }
    }
    if (i != 3)
        return false;
    else
        return true;
}

Vertex *Triangle::vertex(int idx) {
    return this->edges[idx]->vertex[0];
}
