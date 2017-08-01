#ifndef COMBINEDEXPORTER_H_INCLUDED
#define COMBINEDEXPORTER_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>

#include "CombinedPolygon.hpp"
#include "TriangleSpace.h"
#include "ObjectCollection.h"

using namespace std;

class CombinedIO{
    public:
    static int exportBinary(ofstream& fout, std::vector<Vertex*>& v_list);
    static int exportBinary(ofstream& fout, std::vector<CombinedPolygon*>& polygon_list);
    static int exportBinary(ofstream& fout, vector<TriangleSpace*>& ts);

    static int importBinary(ifstream& fin, OBJCollection* objcl);
    static int importBinary(ifstream& fin, vector<Vertex*>& vertex, TriangleSpace* ts);
    static int importBinary(ifstream& fin, vector<Vertex*>& vertex, CombinedPolygon* cp);
};

#endif // COMBINEDEXPORTER_H_INCLUDED
