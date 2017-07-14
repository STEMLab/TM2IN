#include "Trinagle.h"

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

    Point_3 p3a(va->x,va->y,va->z);
    Point_3 p3b(vb->x,vb->y,vb->z);
    Point_3 p3c(vc->x,vc->y,vc->z);

    if (CGAL::collinear(p3a, p3b, p3c)){
        this->normal == CGAL::NULL_VECTOR;
    }
    else{
        this->normal = CGAL::unit_normal(p3a,p3b,p3c);
    }

    return this->normal;
}

std::string Triangle::toJSON(){
    return NULL;
}
