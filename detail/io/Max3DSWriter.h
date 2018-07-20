//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_MAX3DSWRITER_H
#define TM2IN_MAX3DSWRITER_H


#include <cstdio>
#include <features/TriangleMesh.h>

namespace TM2IN {
    namespace detail {
        namespace io {
            class Max3DSWriter {
            public:
                Max3DSWriter(FILE* pFile);
                int write(vector<TriangleMesh*> meshes);
            private:
                FILE* pFile;
            };
        }
    }
}

#endif //TM2IN_MAX3DSWRITER_H
