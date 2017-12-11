#include "model/Triangle.h"

#include "predefine.h"
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
std::string Triangle::toJSON(){
    return NULL;
}

bool Triangle::isAdjacent(Triangle& tri){
    if (isOpposite(tri))return false;
    if (a == tri.a){
        if (b == tri.c) return true;
        else if (c == tri.b) return true;
    }
    else if (a == tri.b){
        if (b == tri.a || c == tri.c) return true;
    }
    else if (a == tri.c){
        if (b == tri.b || c == tri.a) return true;
    }
    else if (c == tri.a){
        if (b == tri.b) return true;
    }
    else if (c == tri.c){
        if (b == tri.a) return true;
    }
    else if (c == tri.b){
        if (b == tri.c) return true;
    }
    return false;

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
