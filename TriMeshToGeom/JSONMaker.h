#ifndef JSONMAKER_H_INCLUDED
#define JSONMAKER_H_INCLUDED

#include <vector>
#include <string>

#include "CombinedPolygon.hpp"

using namespace std;

class JSONMaker{
public :
    static int printJSON(vector<pair<string, vector<CombinedPolygon*>>>& cp, string& filename);
};

#endif // JSONMAKER_H_INCLUDED
