#include "model/Trinagle.h"

Triangle::Triangle()
{
    //ctor
}

Triangle::Triangle(Vertex* pa, Vertex *pb, Vertex* pc)
{
    a = pa;
    b = pb;
    c = pc;
}

Vector_3 Triangle::getNormal()
{
    if (this->normal != CGAL::NULL_VECTOR) return this->normal;

    Vertex* va = this->a;
    Vertex* vb = this->b;
    Vertex* vc = this->c;

    this->normal = CGALCalculation::getUnitNormal(va, vb, vc) * 100 * this->getArea();
    return this->normal;
}

double Triangle::getArea(){
    if (this->area > 0.0) return this->area;
    Vertex* va = this->a;
    Vertex* vb = this->b;
    Vertex* vc = this->c;

    this->area = CGALCalculation::getSquaredArea(va, vb, vc);
    return this->area;
}
std::string Triangle::toJSON(){
    return NULL;
}
