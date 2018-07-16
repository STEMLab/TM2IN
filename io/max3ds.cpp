//
// Created by dongmin on 18. 7. 16.
//

#include <features/TriangleMesh.h>
#include <detail/io/ColladaReader.h>
#include <detail/io/Max3DSReader.h>
#include "max3ds.h"


namespace TM2IN{
    namespace io{
        std::vector<TriangleMesh *> import3DS(const char *filePath) {
            FILE *l_file;
            assert( (l_file=fopen (filePath, "rb") ) != NULL);

            TM2IN::detail::io::Max3DSReader cr(l_file);
            return cr.read();
        }
    }
}
