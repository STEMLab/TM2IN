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
#include <fileio/import/ThreeDSImporter.h>

void TriangleMesh::makeGraph(){
    this->graph = new TriangleMeshGraph();
    this->graph->makeAdjacentGraph(this->triangles);
}

bool TriangleMesh::checkClosedSurface() {
    if (this->graph == NULL || this->graph->isEmpty()){
        cerr << "You didn't make graph." << endl;
    }
    bool isClosed = this->graph->isClosedTriangleMesh();
    if (!isClosed) return false;
    return true;
}

int TriangleMesh::groupByClosedSurface(vector<TriangleMesh *>& new_mesh_list) {
    vector<vector<ull>> cc = this->graph->getConnectedComponent();
    if (cc.size() == 1) {
        new_mesh_list.push_back(this);
        return 0;
    }
    sort(cc.begin(), cc.end(), [](const std::vector< ull >& a, const std::vector< ull >& b){ return a.size() > b.size(); } );
    for (int i = 0 ; i < cc.size() ; i++) {
        TriangleMesh* new_mesh = new TriangleMesh();
        new_mesh->name = this->name +"_new_"+std::to_string(i);
        for (int j = 0 ; j < cc[i].size() ; j++){
            new_mesh->triangles.push_back(this->triangles[cc[i][j]]);
        }
        new_mesh->updateVertexByTriangleList();
        new_mesh_list.push_back(new_mesh);
    }
    delete this;
    return 1;
}

bool TriangleMesh::resolveWrongTriangle() {
    for (int j = 0 ; j < this->triangles.size() ; j++){
        Triangle* triangle = this->triangles[j];

        Vector_3 triangleNormal = triangle->getNormal();

        if (triangleNormal == CGAL::NULL_VECTOR){
            for (int k = 0 ; k < 2; k ++){
                for (int w = k + 1; w < 3 ; w ++){
                    if (Checker::isSameVertex(triangle->vertex(k), triangle->vertex(w))){
                        this->vertices[triangle->vertex(k)->index] = triangle->vertex(w);
                        this->triangles.erase(this->triangles.begin() + j);
                        cerr << "Same Vertex In One Triangle" << endl;
                        return true;
                    }
                }
            }
            if (Checker::isCollinear(triangle->vertex(0), triangle->vertex(1), triangle->vertex(2))) {
                cerr << "Collinear Vertices in Triangle" << endl;
                this->triangles.erase(this->triangles.begin() + j);
                return true;
            }
            cerr << "Why you are here"<< endl;
            assert(triangleNormal != CGAL::NULL_VECTOR);
        }
    }

    return false;
}

void TriangleMesh::clear(){
    triangles.clear();
    vertices.clear();
    graph->clear();
}


/*
void TriangleMesh::exportTVR(const char *path) {
    std::cout << "save As TVR" << endl;
    cout << path;
    FILE* pFile;
    pFile= fopen(path, "w");
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
*/

void TriangleMesh::updateVertexByTriangleList() {
    for (int i = 0 ; i < this->triangles.size() ; i++){
        for (int vt = 0 ; vt < 3; vt++){
            int j;
            for (j = 0 ; j < this->vertices.size() ; j++){
                if (this->vertices[j] == this->triangles[i]->vertex(vt)){
                    break;
                }
            }
            if (j == this->vertices.size()){
                this->vertices.push_back(this->triangles[i]->vertex(vt));
            }
        }

    }
}
