#ifndef JSONMAKER_H_INCLUDED
#define JSONMAKER_H_INCLUDED

#include <vector>
#include <string>

#include "data/Surface.hpp"
#include "data/TriangleSpace.h"

using namespace std;

class JSONMaker{
public :
    static int printJSON(ofstream& fout, std::vector<Surface*>& polygon_list);
    static int printJSON(ofstream& fout, vector<TriangleSpace*>& ts);
};

#endif // JSONMAKER_H_INCLUDED
