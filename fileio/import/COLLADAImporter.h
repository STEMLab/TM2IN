//
// Created by dongmin on 18. 3. 23.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_COLLADAIMPORTER_H
#define TRIANGLEMESHTOCLEARSOLID_COLLADAIMPORTER_H

#include "Importer.h"
#include "rapidxml/rapidxml.hpp"

class COLLADAImporter : public Importer {
public:
    COLLADAImporter();
    virtual ~COLLADAImporter();
    TriangleMesh* import(const char *filePath);
private:
    string queryAttributeValueInNodes(rapidxml::xml_node<> *pNode, const char *childNodeName,
                                          const char *condAttributeName, const char *condAttributeValue,
                                          const char *resultAttributeName);
};


#endif //TRIANGLEMESHTOCLEARSOLID_COLLADAIMPORTER_H
