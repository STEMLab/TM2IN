#ifndef TRIANGLEMESH_H_INCLUDED
#define TRIANGLEMESH_H_INCLUDED

#include <vector>
#include <string>
#include "predefine.h"
#include "features/Triangle.h"
#include "features/TriangleMeshGraph.h"
using namespace std;

class TriangleMesh{
private:
public:
    vector<pair<string, vector<Triangle*>>> triangles;
    vector<Vertex*> vertices;
    vector<TriangleMeshGraph*> graphs;

    TriangleMesh(){}

    void makeGraph();
    void exportTVR(const char *);
    void export3DS(const char* filePath);
    bool checkClosedSurface();

    int groupByClosedSurface();

    bool resolveWrongTriangle();
};

#endif // TRIANGLEMESH_H_INCLUDED
