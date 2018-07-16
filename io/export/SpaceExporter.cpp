//
// Created by dongmin on 18. 3. 13.
//

#include <fstream>
#include <fileio/JSONMaker.h>
#include "SpaceExporter.h"

int SpaceExporter::exportJSON(vector<Solid *> spaceList, const char *filePath) {
    ofstream fout;
    fout.open(filePath, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printJSON(fout, spaceList)) return -1;
    fout.close();
    return 0;
}
