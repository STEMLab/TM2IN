//
// Created by dongmin on 18. 8. 9.
//

#include <detail/features/halfedge_string.h>
#include <algorithm/triangulation.h>
#include <Options.h>
#include <features/HalfEdge.h>
#include "features/Wall/Triangle.h"
#include "cgal/triangle.h"
#include "TriangulatedSurface.h"

namespace TM2IN {
    namespace Wall {
        TriangulatedSurface::TriangulatedSurface() {
            type = TM2IN::IND_TYPE::TriangulatedSurface;
        }

        TriangulatedSurface::TriangulatedSurface(TriangulatedSurface *pSurface) {
            this->exteriorBoundary = pSurface->getExteriorBoundary();
            TM2IN::detail::HalfEdgeString::setParent(this->exteriorBoundary, this);
            this->normal = pSurface->normal;
            //this->mbb = new MinimumBoundingBox(pSurface->getMBB());
            this->mbb = pSurface->getMBB();
            this->area = pSurface->getArea();
            this->triangles = pSurface->triangles;
            this->geom_id = pSurface->geom_id;
            this->type = TM2IN::IND_TYPE::TriangulatedSurface;
            this->surface_type = pSurface->surface_type;
        }

        TriangulatedSurface::TriangulatedSurface(Triangle& pl){
            this->geom_id = "sf" + pl.geom_id;
            this->exteriorBoundary = pl.getExteriorBoundary();
            TM2IN::detail::HalfEdgeString::setParent(this->exteriorBoundary, this);

            area = pl.getArea();
            normal = pl.getNormal();

            // this->updateMBB();
            this->mbb = pl.getMBB();
            this->triangles.push_back(&pl);
            this->type = TM2IN::IND_TYPE::TriangulatedSurface;
            this->surface_type = pl.surface_type;
        }

        bool TriangulatedSurface::strict_validation(){
            if (!easy_validation()) {
                return false;
            }
            return this->is_simple();
        }

        bool TriangulatedSurface::easy_validation(){
            if (this->getVerticesSize() < 3) {
                return false;
            }
            return !this->has_duplicate_vertex();
        }

        bool TriangulatedSurface::updateNormal() {
            if (this->getVerticesSize() <= 4){
                Vector_3 normal = Vector_3(0,0,0);
                for (int i = 0 ; i < (int) this->getVerticesSize() - 1 ; i += 2){
                    int e_i = i + 2 >= (int) this->getVerticesSize()? 0 : i+2;
                    normal = normal + TM2IN::cgal::triangle_normal(vertex(i), vertex(i + 1), vertex(e_i));
                }
                this->normal = normal;
            }
            if (Options::getInstance()->do_validation)
                assert(this->normal != CGAL::NULL_VECTOR);

            return true;
        }

        std::vector<Wall::TriangulatedSurface *> TriangulatedSurface::getNeighborList() {
            std::vector<Wall::TriangulatedSurface *> neighbors;
            for (HalfEdge* he : this->exteriorBoundary){
                if (he->oppositeEdge != NULL && he->oppositeEdge->parent != NULL)
                    neighbors.push_back((Wall::TriangulatedSurface *)he->oppositeEdge->parent);
            }
            return neighbors;
        }
    }
}