//
// Created by dongmin on 18. 7. 25.
//

#include <detail/cgal/geometry.h>
#include "config.h"
#include "util.h"
#include "features/Triangle.h"
#include "features/RoomBoundary/TriangleMesh.h"
#include "features/Room.h"

#include "RoomFactory.h"

namespace TM2IN {
    namespace detail {
        std::vector<Vertex *> &RoomFactory::getVerticesList() {
            return this->raw_vertices;
        }

        void RoomFactory::setRoomName(std::string str) {
            this->room_name = str;
        }

        RoomFactory::RoomFactory() {

        }

        void RoomFactory::pushVertex(Vertex *vt) {
            this->raw_vertices.push_back(vt);
        }

        void RoomFactory::pushTriangle(Triangle *tri) {
            int neighbor_num = 0;
            for (ull i = 0 ; i < this->raw_triangles.size() ; i++){
                if (TM2IN::detail::cgal::has_bbox_intersect(raw_triangles[i], tri))
                    if (raw_triangles[i]->setNeighbor(tri))
                        neighbor_num++;
            }
            assert(neighbor_num <= 3);
            this->raw_triangles.push_back(tri);
        }

        std::vector<Room *> RoomFactory::make() {
            RoomBoundary::TriangleMesh* boundary = new RoomBoundary::TriangleMesh(this->raw_triangles);
            boundary->makeGraph();

            if (!boundary->is_closed_triangle_mesh()){
                throw std::runtime_error("Triangle Mesh is not closed or wrong");
            }

            std::vector<RoomBoundary::TriangleMesh*> boundaries = paritionRoom(boundary);
            int i;
            switch(options->selected){
                case ARCH:
                    i = 0;
                    while (i < boundaries.size()){
                        if (is_furniture(boundaries[i]))
                            boundaries.erase(boundaries.begin() + i);
                        else
                            i++;
                    }
                    break;
                case NON_ARCH:
                    i = 0;
                    while (i < boundaries.size()){
                        if (is_furniture(boundaries[i]))
                            i++;
                        else
                            boundaries.erase(boundaries.begin() + i);
                    }
                    break;
                default:
                    break;
            }

            vector<Room*> rooms;
            for (RoomBoundary::TriangleMesh* tm : boundaries){
                rooms.push_back(new Room(tm));
            }

            this->raw_triangles.clear();
            if (this->keep_vertices){

            }
            else{
                this->raw_vertices.clear();
            }
        }

        vector<TriangleMesh *> RoomFactory::paritionRoom(TriangleMesh *pMesh) {
            return vector<TriangleMesh *>();
        }

        bool RoomFactory::is_furniture(TriangleMesh *&tm) {
            return false;
        }
    }
}