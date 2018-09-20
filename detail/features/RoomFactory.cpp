//
// Created by dongmin on 18. 7. 25.
//

#include <detail/cgal/geometry.h>
#include <cgal/vector_angle.h>
#include "config.h"
#include "util.h"
#include "features/Wall/Triangle.h"
#include "features/RoomBoundary/TriangleMesh.h"
#include "features/Room.h"
#include "Options.h"
#include "features/Vertex.h"

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
            vt->geom_id = to_string(this->raw_vertices.size());
            this->raw_vertices.push_back(vt);
        }

        void RoomFactory::pushTriangle(Wall::Triangle *tri) {
            int neighbor_num = 0;
            for (ull i = 0 ; i < this->raw_triangles.size() ; i++){
                if (TM2IN::detail::cgal::has_bbox_intersect(raw_triangles[i], tri))
                    if (raw_triangles[i]->setNeighbor(tri))
                        neighbor_num++;
            }
            assert(neighbor_num <= 3);
            tri->geom_id = to_string(this->raw_triangles.size());
            this->raw_triangles.push_back(tri);
        }

        /**
         * @todo MANUAL
         * @return
         */
        std::vector<Room *> RoomFactory::make() {
            RoomBoundary::TriangleMesh* boundary = new RoomBoundary::TriangleMesh(this->raw_triangles);

            if (!boundary->is_closed_triangle_mesh()){
                throw std::runtime_error("Triangle Mesh is not closed or wrong");
            }

            std::vector<RoomBoundary::TriangleMesh*> boundaries = paritionRoom(boundary);
            int i;
            switch(Options::getInstance()->selected){
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
                case MANUAL:
                    i = 0;
                    while (i < boundaries.size()){
                        i++;
                    }
                    break;
                default:
                    break;
            }

            vector<Room*> rooms;
            for (RoomBoundary::TriangleMesh* tm : boundaries){
                rooms.push_back(new Room(tm));
            }

            if (rooms.size() == 1){
                rooms[0]->name = room_name;
            }
            else{
                for (int ri = 0 ; ri < rooms.size() ; ri++){
                    rooms[ri]->name = room_name + "_" + to_string(ri);
                }
            }

            this->raw_triangles.clear();
            if (!this->keep_vertices){
                this->raw_vertices.clear();
            }

            return rooms;
        }

        vector<TriangleMesh *> RoomFactory::paritionRoom(TriangleMesh *pMesh) {
            vector<TriangleMesh*> results = pMesh->disjoint_meshes();
            return results;
        }

        bool RoomFactory::is_furniture(TriangleMesh *&tm) {
            vector<Triangle_3> cgal_triangles;
            vector<Wall::Triangle*> triangles = tm->getTriangleList();
            for (int i = 0 ; i < triangles.size() ; i++){
                cgal_triangles.push_back(triangles[i]->CGAL_triangle());
            }

            int innerCount = 0, outerCount = 0;
            Bbox_3 bbox3 = CGAL::bbox_3(cgal_triangles.begin(), cgal_triangles.end());

            int index = 0;
            std::random_device rd;
            std::mt19937 gen(rd());

            while (innerCount < 10 && outerCount < 10){
                double coords[3];
                double outsideValue;
                std::uniform_real_distribution<> dis( 0, bbox3.max(index%3) - bbox3.min(index % 3));
                if (index % 6 >= 3){
                    outsideValue = bbox3.max(index % 3) + dis(gen);
                }
                else{
                    outsideValue = bbox3.min(index % 3) - dis(gen);
                }
                coords[index % 3] = outsideValue;
                int next1 = (index + 1) % 3;
                int next2 = (index + 2) % 3;
                std::uniform_real_distribution<> dis1 (bbox3.min(next1), bbox3.max(next1));
                std::uniform_real_distribution<> dis2 (bbox3.min(next2), bbox3.max(next2));

                coords[next1] = dis1(gen);
                coords[next2] = dis2(gen);

                Point_3 outsidePoint(coords[0], coords[1], coords[2]);
                assert(outsidePoint[0] > bbox3.xmax() || outsidePoint[0] < bbox3.xmin() ||
                       outsidePoint[1] > bbox3.ymax() || outsidePoint[1] < bbox3.ymin() ||
                       outsidePoint[2] > bbox3.zmax() || outsidePoint[2] < bbox3.zmin());

                double minDist = INT_MAX;
                int minIndex = -1;
                for (int i = 0 ; i < cgal_triangles.size() ; i++){
                    Triangle_3& cgal_triangle = cgal_triangles[i];
                    double dist = CGAL::squared_distance(cgal_triangle, outsidePoint);
                    if (minDist > dist){
                        minDist = dist;
                        minIndex = i;
                    }
                }

                Vector_3 normal_of_triangle = cgal_triangles[minIndex].supporting_plane().orthogonal_vector();
                double vector_coords[3] = {0, 0, 0};
                vector_coords[index % 3] = index % 6 >= 3? -1 : 1;
                Vector_3 vector_to_min_plane(vector_coords[0], vector_coords[1], vector_coords[2]);

                double angle = TM2IN::cgal::getAngle(normal_of_triangle, vector_to_min_plane);
                assert(angle != 90.0);
                if (angle > 90 && angle <= 180){
                    //direction to outside
                    outerCount++;
                }
                else if (angle < 90 && angle >= 0){
                    //direction to inside
                    innerCount++;
                }
                else{
                    throw std::runtime_error("is Furniture Error");
                }
            }
            if (innerCount > 0 && outerCount > 0){
                cerr << "\nSomething wrong in is Furniture" << endl;
                cerr << "innerCount : " << innerCount << endl;
                cerr << "outerCount : " << outerCount << "\n" << endl;
            }
            if (outerCount > innerCount){
                return true;
            }
            else if (innerCount > outerCount)
                return false;
        }
    }
}