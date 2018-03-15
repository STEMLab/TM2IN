//
// Created by dongmin on 18. 1. 17.
//

#include "features/TriangleMesh.h"
#include <utility>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <logic/check.hpp>

void TriangleMesh::makeGraph(){
    for (int i = 0 ; i < this->triangles.size() ; i++){
        TriangleMeshGraph* graph = new TriangleMeshGraph();
        graph->makeAdjacentGraph(this->triangles[i].second);
        this->graphs.push_back(graph);
    }

}

void TriangleMesh::saveAsTVR(const char *path) {
    std::cout << "save As TVR" << endl;
    cout << path;
    pFile = fopen(path, "w");
    fprintf(pFile, "TVR0\n");

    for (int i = 0 ; i < this->vertices.size() ; i++){
        ostringstream strStream;
        strStream << "v ";
        for (int j = 0 ; j < 3 ; j++){
            strStream << this->vertices[i]->coords[j] << " ";
        }
        strStream << endl;
        string str = strStream.str();
        fprintf(pFile, "%s", str.c_str());
    }

    for (int group_i = 0 ; group_i < this->triangles.size() ; group_i++){
        fprintf(pFile, "g %s\n", this->triangles[group_i].first.c_str());
        for (int tri_i = 0 ; tri_i < this->triangles[group_i].second.size(); tri_i++){
            Triangle* triangle = this->triangles[group_i].second[tri_i];
            ostringstream strStream;
            strStream << "f ";
            for (int j = 0 ; j < 3 ; j++){
                strStream << triangle->vertex(j)->index << " ";
            }
            strStream << endl;
            fprintf(pFile,"%s", strStream.str().c_str());
        }
    }

    fclose(pFile);

}

bool TriangleMesh::checkClosedSurface() {
    for (int groupI = 0 ; groupI < this->graphs.size() ; groupI++){
        bool isClosed = this->graphs[groupI]->isClosedTriangleMesh();
        if (!isClosed) return false;
        cout << "group " << groupI << " is composed of closed Triangle Mesh" << endl;
    }
    return true;
}

int TriangleMesh::groupByClosedSurface() {
    vector<pair<string, vector<Triangle*> > > newTriangles;
    int index = 0;
    for (int groupI = 0 ; groupI < this->graphs.size() ; groupI++){
        vector<vector<ull>> cc = this->graphs[groupI]->getConnectedComponent();
        if (cc.size() == 1) continue;

        // Descending Order
        sort(cc.begin(), cc.end(), [](const std::vector< ull >& a, const std::vector< ull >& b){ return a.size() > b.size(); } );
        for (int i = 0 ; i < cc.size() ; i++){
            string name = "new" + std::to_string(index++);
            vector<Triangle*> triangles;
            for (int j = 0 ; j < cc[i].size() ; j++){
                triangles.push_back(this->triangles[groupI].second[cc[i][j]]);
            }
            newTriangles.push_back(make_pair(name, triangles));
        }
    }
    this->triangles = newTriangles;
    return 0;
}

bool TriangleMesh::resolveWrongTriangle() {
    for (int i = 0 ; i < this->triangles.size() ; i++){
        for (int j = 0 ; j < this->triangles[i].second.size() ; j++){
            Triangle* triangle = this->triangles[i].second[j];

            Vector_3 triangleNormal = triangle->getNormal();

            if (triangleNormal == CGAL::NULL_VECTOR){
                for (int k = 0 ; k < 2; k ++){
                    for (int w = k + 1; w < 3 ; w ++){
                        if (Checker::isSameVertex(triangle->vertex(k), triangle->vertex(w))){
                            this->vertices[triangle->vertex(k)->index] = triangle->vertex(w);
                            this->triangles[i].second.erase(this->triangles[i].second.begin() + j);
                            cerr << "Same Vertex In One Triangle" << endl;
                            return true;
                        }
                    }
                }
                if (Checker::isCollinear(triangle->vertex(0), triangle->vertex(1), triangle->vertex(2))) {
                    cerr << "Collinear Vertices in Triangle" << endl;
                    this->triangles[i].second.erase(this->triangles[i].second.begin() + j);
                    return true;
                }
                cerr << "Why you are here"<< endl;
                assert(triangleNormal != CGAL::NULL_VECTOR);
            }
        }
    }
    return false;
}
