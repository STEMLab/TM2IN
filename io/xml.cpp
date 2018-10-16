//
// Created by dongmin on 18. 7. 16.
//

#include "xml.h"

#include <vector>
#include <rapidxml/rapidxml.hpp>
#include <fstream>
#include <detail/io/ColladaReader.h>
#include <detail/io/IndoorGMLWriter.h>
#include <detail/io/InFactoryClient.h>

using namespace rapidxml;

namespace TM2IN{
    namespace io{
        std::vector<Room *> importDAE(const char *filePath) {
            ifstream pFile(filePath);

            TM2IN::detail::io::ColladaReader cr(pFile);
            return cr.read();
        }

        int exportIndoorGML_infactory(const char *filePath, std::vector<Room *> rooms) {
            ofstream pFile(filePath);
            TM2IN::detail::io::InFactoryClient ic(rooms);
            ic.make();
            std::string xml_string = ic.getDocumentStr();
            pFile << xml_string;
            pFile.close();
            return 0;
        }

        int exportIndoorGML(const char *filePath, std::vector<Room *> rooms) {
            std::ofstream pFile(filePath);

            TM2IN::detail::io::IndoorGMLWriter writer(pFile);
            return writer.write(rooms);
        }


    }
}
