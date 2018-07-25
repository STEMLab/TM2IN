#ifndef TRIANGLEMESH_H_INCLUDED
#define TRIANGLEMESH_H_INCLUDED

#include <vector>
#include <string>

#include "features/Geometry.h"
#include "TriangleMeshGraph.h"
#include "Vertex.h"
#include "Triangle.h"

using namespace std;

namespace TM2IN{
    class TriangleMesh : public Geometry{
    public:
        string name;
        vector<Triangle*> triangles;
        vector<Vertex*> vertices;
        TriangleMeshGraph* graph;

        TriangleMesh(){}

        void init();
        bool checkClosed();
        int partitionByComponent(vector<TriangleMesh *> &new_mesh_list);
        bool isFurniture();

        std::string asJsonText();
        void updateMBB();
    private:
        void makeGraph();

        void updateVertexByTriangleList();
    };

}

#endif // TRIANGLEMESH_H_INCLUDED
