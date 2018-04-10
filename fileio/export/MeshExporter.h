//
// Created by dongmin on 18. 3. 30.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_THREEDSEXPORTER_H
#define TRIANGLEMESHTOCLEARSOLID_THREEDSEXPORTER_H


#include <features/TriangleMesh.h>

using namespace std;

class MeshExporter{
public:
    static int export3DS(vector<TriangleMesh *> meshList, const char *filePath);

};


#endif //TRIANGLEMESHTOCLEARSOLID_THREEDSEXPORTER_H
