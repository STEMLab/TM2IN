//
// Created by dongmin on 18. 3. 23.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_COLLADAIMPORTER_H
#define TRIANGLEMESHTOCLEARSOLID_COLLADAIMPORTER_H

#include "fileio/MeshImporter.h"

class COLLADAImporter : public MeshImporter {
public:
    COLLADAImporter();
    virtual ~COLLADAImporter();
    vector<TriangleMesh*> import(const char *filePath);
private:

};


#endif //TRIANGLEMESHTOCLEARSOLID_COLLADAIMPORTER_H
