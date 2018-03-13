//
// Created by dongmin on 18. 3. 13.
//

#ifndef TRIANGLEMESHTOCLEARSOLID_THREEDSEXPORTER_H
#define TRIANGLEMESHTOCLEARSOLID_THREEDSEXPORTER_H

#include "Exporter.h"

using namespace std;

class ThreeDSExporter : public Exporter{
public:
    int exportSpace(vector<Space*> spaceList, const char* name);
};


#endif //TRIANGLEMESHTOCLEARSOLID_THREEDSEXPORTER_H
