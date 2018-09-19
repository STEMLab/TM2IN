//
// Created by dongmin on 18. 7. 18.
//

#ifndef TM2IN_JSONWRITER_H
#define TM2IN_JSONWRITER_H

#include <fstream>
#include "features/IndoorComponent.h"

using namespace std;

namespace TM2IN {
    namespace detail {
        namespace io {
            /**
             * @ingroup imp_details
             */
            class JsonWriter {
            public :
                JsonWriter(std::ofstream& _fout);
                void write(vector<Room *> &ts, int boundary_mode);
            private:
                std::ofstream& fout;
                bool exp_tri;
            };

            string room_to_json(Room &ps, int boundary_mode);
            string surface_to_json(Wall::Surface& sf);
            string surface_to_json_with_triangles(Wall::TriangulatedSurface &pSurface);
            string vertex_to_json(Vertex& vt, string index);
        }
    }
}


#endif //TM2IN_JSONWRITER_H
