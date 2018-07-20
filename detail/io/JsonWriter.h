//
// Created by dongmin on 18. 7. 18.
//

#ifndef TM2IN_JSONWRITER_H
#define TM2IN_JSONWRITER_H

#include <fstream>

#include "features/PolyhedralSurface.h"


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

            string to_json(PolyhedralSurface* ps, bool exp_tri=false);
            string to_json(Surface* sf);
            string to_json_with_triangles(Surface *pSurface);
            string to_json(Vertex& vt, int index = 0);
        }
    }
}


#endif //TM2IN_JSONWRITER_H
