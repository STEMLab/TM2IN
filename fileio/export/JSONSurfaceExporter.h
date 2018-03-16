//
// Created by dongmin on 18. 3. 13.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_JSONSURFACEEXPORTER_H
#define TRIANGLEMESHTOCLEARSOLID_JSONSURFACEEXPORTER_H

#include "Exporter.h"

using namespace std;

class JSONSurfaceExporter : public Exporter{
public:
    int exportSpace(vector<Space *> spaceList, const char *filePath);
};


#endif //TRIANGLEMESHTOCLEARSOLID_JSONSURFACEEXPORTER_H
