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
    vector<pair<string, vector<Triangle>>> triangles;
    vector<Vertex*> vertices;
    TriangleMeshGraph* graph;

    TriangleMesh(){}

    TriangleMesh* correct();
    void makeGraph();
    void saveFile(const char*);
    bool checkValid();
};

#endif // TRIANGLEMESH_H_INCLUDED
