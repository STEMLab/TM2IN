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
        class RoomFactory {
        private:
            std::vector<Vertex*> raw_vertices;
            std::vector<Triangle*> raw_triangles;
            std::string room_name;
        public:
            RoomFactory();
            void pushVertex(Vertex* vt);
            void pushTriangle(Triangle *tri);
            std::vector<Room*> make();

            bool keep_vertices = false;

            std::vector<Vertex *> &getVerticesList();

            void setRoomName(std::string str);

            std::vector<TriangleMesh *> paritionRoom(TriangleMesh *pMesh);

            bool is_furniture(TriangleMesh *&tm);
        };
    }
}


#endif //TM2IN_ROOMFACTORY_H
