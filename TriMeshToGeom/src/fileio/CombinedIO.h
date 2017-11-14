#ifndef COMBINEDEXPORTER_H_INCLUDED
#define COMBINEDEXPORTER_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>

#include "data/Surface.hpp"
#include "data/Space.h"
#include "data/ObjectCollection.h"

using namespace std;

class CombinedIO{
    public:
    static int exportBinary(ofstream& fout, std::vector<Vertex*>& v_list);
    static int exportBinary(ofstream& fout, std::vector<Surface*>& polygon_list);
    static int exportBinary(ofstream& fout, vector<Space*>& ts);

    static int importBinary(ifstream& fin, OBJCollection* objcl);
    static int importBinary(ifstream& fin, vector<Vertex*>& vertex, Space* ts);
    static int importBinary(ifstream& fin, vector<Vertex*>& vertex, Surface* cp);
};

#endif // COMBINEDEXPORTER_H_INCLUDED
