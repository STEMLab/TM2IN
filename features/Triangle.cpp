#include "features/Triangle.h"
#include "features/HalfEdge.h"

Triangle::Triangle(Vertex* pa, Vertex *pb, Vertex* pc)
{
    edges.push_back(new HalfEdge(pa, pb, this));
    edges.push_back(new HalfEdge(pb, pc, this));
    edges.push_back(new HalfEdge(pc, pa, this));
}

void Triangle::setNormal(Vector_3 vec){
    this->normal = vec;
    CGAL_assertion(this->normal != CGAL::NULL_VECTOR);
}

Vector_3 Triangle::getNormal()
{
    CGAL_assertion(this->normal != CGAL::NULL_VECTOR);
    return this->normal;
}

void Triangle::setArea(double _area){
    this->area = _area;
    CGAL_assertion(this->area != 0.0);
};

double Triangle::getArea(){
    CGAL_assertion(this->area != 0.0);
    return this->area;
}

int Triangle::findShareSameHalfEdge(Triangle *pTriangle){
    int result = -1;
    for (int i = 0 ; i < 3 ; i++){
        for (int j = 0 ; j < 3 ; j++) {
            if (this->edges[i]->isSame(pTriangle->edges[j])){
                if (result == -1) result = i;
                else {
                    std::cerr << "already share same edges" << std::endl;
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
                if (i == 3) return true;
                if (this->vertex(i) != tri->vertex(j)){
                    return false;
                }

            }
        }
    }

    return false;

}

Vertex *Triangle::vertex(int idx) {
    return this->edges[idx]->vertices[0];
}

std::string Triangle::toJSON(std::string &indent) {
    std::string ret;
    ret += indent + "{\n";

    //normal
    ret += indent + "\"normal\" : [";
    Vector_3 normal = getNormal();
    ret += indent + std::to_string(normal.x()) + ", " + std::to_string(normal.y()) + ", " + std::to_string(normal.z());
    ret += indent + "],\n";

    //coordinates
    ret += indent + "\"coord\" : [\n";
    for (int i = 0 ; i < 3 ; i++){
        ret += indent + "\t";
        ret += this->vertex(i)->toJSON();
        if (i != 2)
            ret += ",";
        ret += "\n";
    }
    ret += indent + "]}";

    return ret;
}

void Triangle::init() {
    double triangleArea = CGALCalculation::getAreaOfTriangle(*this);
    this->setArea(triangleArea);

    Vector_3 triangleNormal = CGALCalculation::normalVector(*this);
    this->setNormal(triangleNormal);
}
