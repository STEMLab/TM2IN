//
// Created by dongmin on 18. 3. 13.
//

#include <fstream>
#include <fileio/JSONMaker.h>
#include "JSONSurfaceExporter.h"

int JSONSurfaceExporter::exportSpace(vector<Space *> spaceList, const char *name) {
    ofstream fout;
    fout.open(name, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printJSON(fout, spaceList)) return -1;
    fout.close();
    return 0;
}
