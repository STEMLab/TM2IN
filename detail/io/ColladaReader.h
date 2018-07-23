//
// Created by dongmin on 18. 7. 16.
//

#ifndef TM2IN_COLLADAREADER_H
#define TM2IN_COLLADAREADER_H

#include <lib/rapidxml/rapidxml.hpp>
#include "features/Geometry.h"

#include <fstream>

using namespace std;

namespace TM2IN {
    namespace detail {
        namespace io {
            class ColladaReader{
            public:
                ColladaReader(ifstream& _ifs);
                std::vector<TriangleMesh *> read();
            private:
                ifstream& ifs;

                std::string queryAttributeValueInNodes(rapidxml::xml_node<> *pNode, const char *childNodeName,
                                                       const char *condAttributeName, const char *condAttributeValue,
                                                       const char *resultAttributeName);
            };
        }
    }
}

#endif //TM2IN_COLLADAREADER_H
