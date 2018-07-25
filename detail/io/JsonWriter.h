//
// Created by dongmin on 18. 7. 18.
//

#ifndef TM2IN_JSONWRITER_H
#define TM2IN_JSONWRITER_H

#include <fstream>
#include <features/PolyhedralSurface.h>

#include "features/Geometry.h"

using namespace std;

namespace TM2IN {
    namespace detail {
        namespace io {
            class JsonWriter {
            public :
                JsonWriter(std::ofstream& _fout);
                void write(vector<PolyhedralSurface *> &ts, bool exp_tri=false);
            private:
                std::ofstream& fout;
                bool exp_tri;
            };

            string polyhedral_surface_to_json(PolyhedralSurface& ps, bool exp_tri=false);
            string surface_to_json(Surface& sf);
            string surface_to_json_with_triangles(Surface &pSurface);
            string vertex_to_json(Vertex& vt, int index = 0);
        }
    }
}


#endif //TM2IN_JSONWRITER_H
