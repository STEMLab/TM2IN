#ifndef Importer_H
#define Importer_H

#include <lib/rapidxml/rapidxml.hpp>
#include "util.h"
#include "features/TriangleMesh.h"
#include <vector>

using namespace std;

class MeshImporter
{
    public:
        static std::vector<TriangleMesh*> import3DS(const char *filePath);
        static vector<TriangleMesh*> importTVR(const char *filePath);
        static vector<TriangleMesh*> importDAE(const char *filePath);
    protected:

    private:
        static string queryAttributeValueInNodes(rapidxml::xml_node<> *pNode, const char *childNodeName,
                                      const char *condAttributeName, const char *condAttributeValue,
                                      const char *resultAttributeName);
};


#endif // Importer_H
