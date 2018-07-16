//
// Created by dongmin on 18. 7. 16.
//

#include "collada.h"

#include <vector>
#include <features/TriangleMesh.h>
#include <lib/rapidxml/rapidxml.hpp>
#include <fstream>
#include <detail/io/ColladaReader.h>

using namespace rapidxml;

namespace TM2IN{
    namespace io{
        std::vector<TriangleMesh *> importDAE(const char *filePath) {
            ifstream pFile(filePath);

            TM2IN::detail::io::ColladaReader cr(pFile);
            return cr.read();
        }

        

    }
}
