#ifndef JSONMAKER_H_INCLUDED
#define JSONMAKER_H_INCLUDED

#include <vector>
#include <string>

#include "features/Surface.h"
#include "features/Space.h"

using namespace std;

class JSONMaker{
public :
    static int printJSON(ofstream& fout, std::vector<Surface*>& polygon_list);
    static int printJSON(ofstream& fout, Space* space);
    static int printJSON(ofstream& fout, vector<Space*>& ts);
};

#endif // JSONMAKER_H_INCLUDED
