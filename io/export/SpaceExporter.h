//
// Created by dongmin on 18. 3. 13.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_JSONSURFACEEXPORTER_H
#define TRIANGLEMESHTOCLEARSOLID_JSONSURFACEEXPORTER_H

#include <features/Solid.h>

using namespace std;

class SpaceExporter{
public:
    static int exportJSON(vector<Solid *> spaceList, const char *filePath);
};


#endif //TRIANGLEMESHTOCLEARSOLID_JSONSURFACEEXPORTER_H
