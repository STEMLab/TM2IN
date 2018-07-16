#ifndef JSONMAKER_H_INCLUDED
#define JSONMAKER_H_INCLUDED

#include <vector>
#include <string>

#include "features/Surface.h"
#include "features/Solid.h"

using namespace std;

class JSONMaker{
public :
    static int printJSON(ofstream& fout, std::vector<Surface*>& surfacesList);
    static int printJSON(ofstream& fout, Solid* space);
    static int printJSON(ofstream& fout, vector<Solid*>& ts);

    static int printTriangleJSON(ofstream& fout, vector<Solid *>& spaces);
    static int printTriangleJSON(ofstream &fout, Solid *space);
    static int printTriangleJSON(ofstream &ofstream, std::vector<Surface *> &surfacesList);
};

#endif // JSONMAKER_H_INCLUDED
