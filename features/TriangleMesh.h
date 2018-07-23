#ifndef TRIANGLEMESH_H_INCLUDED
#define TRIANGLEMESH_H_INCLUDED

#include <vector>
#include <string>

#include "features/Geometry.h"
using namespace std;

class TriangleMesh{
public:
    string name;
    vector<Triangle*> triangles;
    vector<Vertex*> vertices;
    TriangleMeshGraph* graph;

    TriangleMesh(){}

    void init();

    bool checkClosed();

    int partitionByComponent(vector<TriangleMesh *> &new_mesh_list);

    void clear();

    void updateVertexByTriangleList();
    bool isFurniture();
    void makeGraph();
private:
};

#endif // TRIANGLEMESH_H_INCLUDED
