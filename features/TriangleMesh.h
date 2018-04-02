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
    string name;
    vector<Triangle*> triangles;
    vector<Vertex*> vertices;
    TriangleMeshGraph* graph;

    TriangleMesh(){}

    void makeGraph();
    void exportTVR(const char *);
    void export3DS(const char* filePath);
    bool checkClosedSurface();

    int groupByClosedSurface(vector<TriangleMesh *>& new_mesh_list);

    void updateVertexByTriangleList();

    bool resolveWrongTriangle();

    void clear();
};

#endif // TRIANGLEMESH_H_INCLUDED
