#include "model/Triangle.h"

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

bool Triangle::isOpposite(Triangle& tri){
    if (a == tri.a){
        if (b == tri.c)
            if (c == tri.b) return true;
    }
    else if (a == tri.b){
            if (b == tri.a)
                if (c == tri.c) return true;
    }
    else if (a == tri.c){
        if (b == tri.b) if (c == tri.a) return true;
    }
    return false;
}
