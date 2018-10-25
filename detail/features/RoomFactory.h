//
// Created by dongmin on 18. 7. 25.
//

#ifndef TM2IN_ROOMFACTORY_H
#define TM2IN_ROOMFACTORY_H

#include "features/IndoorComponent.h"

using namespace TM2IN::RoomBoundary;
using namespace std;

namespace TM2IN {
    namespace detail {
        /**
         * @ingroup imp_details
         * @brief Input vertices and triangles and make TM2IN::Room
         */
        class RoomFactory {
        private:
            std::vector<Vertex*> raw_vertices;
            std::vector<Wall::Triangle*> raw_triangles;
            std::vector<vector<Vertex*>> raw_triangles_with_vertex;
            std::string room_name;

            void findAndSetNeighbor(vector<long long>& a_triangle, vector<long long>& b_triangle, int i, int a_i, int b_i);
        public:
            RoomFactory();
            void pushVertex(Vertex* vt);
            void pushTriangle(Wall::Triangle *tri);
            void pushTriangle(long long a, long long b, long long c);
            void buildStrTriangle();

            std::vector<Room*> make();

            bool keep_vertices = false;

            std::vector<Vertex *> &getVerticesList();

            void setRoomName(std::string str);

            std::vector<TriangleMesh *> paritionRoom(TriangleMesh *pMesh);

            bool is_furniture(TriangleMesh *&tm);
            void clear();
        };
    }
}


#endif //TM2IN_ROOMFACTORY_H
