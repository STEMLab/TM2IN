//
// Created by dongmin on 18. 7. 16.
//

#include "xml.h"

#include <vector>
#include <lib/rapidxml/rapidxml.hpp>
#include <fstream>
#include <detail/io/ColladaReader.h>
#include <detail/io/IndoorGMLWriter.h>

using namespace rapidxml;

namespace TM2IN{
    namespace io{
        std::vector<Room *> importDAE(const char *filePath) {
            ifstream pFile(filePath);

            TM2IN::detail::io::ColladaReader cr(pFile);
            return cr.read();
        }

        int exportIndoorGML(const char *filePath, std::vector<Room *> rooms) {
            std::ofstream pFile(filePath);

            TM2IN::detail::io::IndoorGMLWriter writer(pFile);
            return writer.write(rooms);
        }


    }
}
