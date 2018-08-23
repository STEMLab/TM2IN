//
// Created by dongmin on 18. 8. 9.
//

#define MIN_TRIANGLE_AREA 0.0000001

#include <Options.h>
#include "Triangle.h"

#include "detail/cgal/type_conversion.h"
#include "features/HalfEdge.h"
#include "cgal/triangle.h"

namespace TM2IN {
    namespace Wall {
        Triangle::Triangle(Vertex *pa, Vertex *pb, Vertex *pc) {
            this->type = IND_TYPE::Triangle;

            exteriorBoundary.push_back(new HalfEdge(pa, pb, this));
            exteriorBoundary.push_back(new HalfEdge(pb, pc, this));
            exteriorBoundary.push_back(new HalfEdge(pc, pa, this));
            Vector_3 _normal = cgal::triangle_normal(pa, pb, pc);
            this->area = CGAL::to_double(sqrt(_normal.squared_length())) / 2;
            if (this->area != 0.0 && this->area == this->area)
                this->normal = _normal;
            else {
                if (Options::getInstance()->do_validation)
                    throw std::runtime_error("create Triangle Error!!");
                else{
                    this->normal = _normal;
                    this->area = MIN_TRIANGLE_AREA;
                }
            }
            this->updateMBB();
        }


        bool Triangle::setNeighbor(Triangle *&tri) {
            bool isAdjacent = false;
            for (int v1 = 0; v1 < 3; v1++) {
                for (int v2 = 0; v2 < 3; v2++) {
                    if (this->exteriorBoundary[v1]->can_be_opposite_edge(tri->exteriorBoundary[v2])) {
                        isAdjacent = true;
                        assert(this->exteriorBoundary[v1]->getOppositeEdge() == NULL);
                        assert(tri->exteriorBoundary[v2]->getOppositeEdge() == NULL);
                        this->exteriorBoundary[v1]->setOppositeEdge(tri->exteriorBoundary[v2]);
                        tri->exteriorBoundary[v2]->setOppositeEdge(this->exteriorBoundary[v1]);
                    }
                }
            }
            return isAdjacent;
        }


        Kernel::Triangle_3 Triangle::CGAL_triangle() {
            return TM2IN::detail::cgal::to_CGAL_Triangle_3(*this);
        }

        bool Triangle::checkNeighbor(Triangle *&tri) {
            for (int v1 = 0; v1 < 3; v1++) {
                if (this->exteriorBoundary[v1]->oppositeEdge != NULL &&
                    this->exteriorBoundary[v1]->oppositeEdge->parent == tri) {
                    return true;
                }
            }
            return false;
        }

        bool Triangle::isOpposite(Triangle *tri) {
            int num_of_opposite = 0;
            for (int i = 0; i < 3; i++) {
                if (this->exterior_boundary_edge(i)->getOppositeEdge() != NULL &&
                    this->exterior_boundary_edge(i)->oppositeEdge->parent == tri) {
                    num_of_opposite++;
                }
                if (num_of_opposite == 2) {
                    std::cerr << "isOpposite worognonrownr" << std::endl;
                    exit(-1);
                }
            }
            return num_of_opposite == 3;

        }

        bool Triangle::updateNormal() {
            this->normal = TM2IN::cgal::triangle_normal(vertex(0), vertex(1), vertex(2));
            return true;
        }
    }
}