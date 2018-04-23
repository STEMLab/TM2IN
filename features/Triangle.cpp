#include "features/Triangle.h"
#include "features/HalfEdge.h"

Triangle::Triangle(Vertex* pa, Vertex *pb, Vertex* pc)
{
    boundaryEdges.push_back(new HalfEdge(pa, pb, this));
    boundaryEdges.push_back(new HalfEdge(pb, pc, this));
    boundaryEdges.push_back(new HalfEdge(pc, pa, this));
    Vector_3 _normal = CGALCalculation::getCrossProduct(pa, pb, pc);
    this->area = CGAL::to_double(sqrt(_normal.squared_length())) / 2;
    if (this->area != 0.0)
        this->normal = _normal;
    this->updateMBB();
}

int Triangle::findShareSameHalfEdge(Triangle *pTriangle){
    int result = -1;
    for (int i = 0 ; i < 3 ; i++){
        for (int j = 0 ; j < 3 ; j++) {
            if (this->boundaryEdges[i]->isSame(pTriangle->boundaryEdges[j])){
                if (result == -1) result = i;
                else {
                    std::cerr << "already share same boundaryEdges" << std::endl;
                    exit(-1);
                }
            }
        }
    }
    return result;
}

Vertex* Triangle::operator[](int idx){
    if (idx < 3 && idx >= 0) return (*boundaryEdges[idx])[0];
    else{
        std::cerr << "Triangle only has three boundaryEdge" << std::endl;
        exit(-1);
    }
}

bool Triangle::checkNeighbor(Triangle *&tri){
    for (int v1 = 0 ; v1 < 3 ; v1++){
        if (this->boundaryEdges[v1]->oppositeEdge != NULL && this->boundaryEdges[v1]->oppositeEdge->parent == tri){
            return true;
        }
    }
    return false;
}

bool Triangle::setNeighbor(Triangle*& tri){
    bool isAdjacent = false;
    for (int v1 = 0 ; v1 < 3 ; v1++){
        for (int v2 = 0 ; v2 < 3 ;v2++){
            if (this->boundaryEdges[v1]->hasOppositeTwoVertex(tri->boundaryEdges[v2])){
                // assert(!isAdjacent);
                isAdjacent = true;
                assert(this->boundaryEdges[v1]->getOppositeEdge() == NULL);
                assert(tri->boundaryEdges[v2]->getOppositeEdge() == NULL);
                this->boundaryEdges[v1]->setOppositeEdge(tri->boundaryEdges[v2]);
                tri->boundaryEdges[v2]->setOppositeEdge(this->boundaryEdges[v1]);
            }
        }
    }
    return isAdjacent;
}

bool Triangle::isOpposite(Triangle* tri){
    int num_of_opposite = 0;
    for (int i = 0 ; i < 3 ; i++){
        if (this->boundary_edges(i)->getOppositeEdge() != NULL && this->boundary_edges(i)->oppositeEdge->parent == tri){
            num_of_opposite++;
        }
        if (num_of_opposite == 2){
            std::cerr << "isOpposite worognonrownr" << std::endl;
            exit(-1);
        }
    }
    if (num_of_opposite <= 1) return false;
    else return true;

}

Vertex *Triangle::vertex(int idx) {
    return this->boundaryEdges[idx]->vertices[0];
}

std::string Triangle::toJSON(const std::string &indent) {
    std::string ret;
    ret += indent + "{\n";

    //normal
    ret += indent + "\"normal\" : [";
    Vector_3 normal = getNormal();
    ret += indent + std::to_string(normal.x()) + ", " + std::to_string(normal.y()) + ", " + std::to_string(normal.z());
    ret += indent + "],\n";

    //area
    ret += indent + "\"area\" : ";
    double area = getArea();
    ret += indent + std::to_string(area);
    ret += "\n";

    //coordinates
    ret += indent + "\"coord\" : [\n";
    for (int i = 0 ; i < 3 ; i++){
        ret += indent + "\t";
        ret += this->vertex(i)->toJSONString();
        if (i != 2)
            ret += ",";
        ret += "\n";
    }
    ret += indent + "]}";

    return ret;
}

bool Triangle::checkOppositeEdge(Triangle *&tri) {
    for (int v1 = 0 ; v1 < 3 ; v1++){
        for (int v2 = 0 ; v2 < 3 ;v2++){
            if (this->boundaryEdges[v1]->hasOppositeTwoVertex(tri->boundaryEdges[v2])){
                return true;
            }
        }
    }
    return false;
}
