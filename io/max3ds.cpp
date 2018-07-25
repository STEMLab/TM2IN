//
// Created by dongmin on 18. 7. 16.
//

#include "features/TriangleMesh.h"
#include "detail/io/ColladaReader.h"
#include "detail/io/Max3DSReader.h"
#include "detail/io/Max3DSWriter.h"
#include "max3ds.h"

#include <iostream>

using namespace std;

namespace TM2IN{
    namespace io{
        std::vector<Room *> import3DS(const char *filePath) {
            FILE *l_file;
            assert( (l_file=fopen (filePath, "rb") ) != NULL);

            TM2IN::detail::io::Max3DSReader reader(l_file);
            return reader.read();
        }


        int export3DS(const char *filePath, vector<TriangleMesh *> meshList) {
            FILE* pFile;
            pFile= fopen(filePath, "w");
            if (!pFile) {
                cerr << "cannot open export3DS";
                return -1;
            }

            TM2IN::detail::io::Max3DSWriter writer(pFile);
            return writer.write(meshList);
        }

    }
}
