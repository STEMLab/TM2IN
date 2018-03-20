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
    this->graphs.clear();
    for (int i = 0 ; i < this->triangles.size() ; i++){
        TriangleMeshGraph* graph = new TriangleMeshGraph();
        graph->makeAdjacentGraph(this->triangles[i].second);
        this->graphs.push_back(graph);
    }

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
            break; //TODO : for TEST
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

void TriangleMesh::export3DS(const char *filePath) {
    FILE* pFile;
    pFile= fopen(filePath, "w");
    if (!pFile) {
        cerr << "cannot open export3DS";
        return;
    }

    unsigned short chunk_id = 0x4d4d;
    unsigned int chunk_length = 0;
    fwrite(&chunk_id, 2, 1, pFile);
    fwrite(&chunk_length, 4, 1, pFile);

    chunk_id = 0x3D3D;
    chunk_length = 0;
    fwrite(&chunk_id, 2, 1, pFile);
    fwrite(&chunk_length, 4, 1, pFile);

    unsigned int wholeLength = 0;
    for (int groupI = 0 ; groupI < this->triangles.size() ; groupI++) {
        vector<Vertex*> vertices;
        vector<vector<int>> triangleIndicies;
        for (int triI = 0 ; triI < this->triangles[groupI].second.size(); triI++){
            Triangle* triangle = this->triangles[groupI].second[triI];
            vector<int> triangleIndex;
            for (int vi = 0 ; vi < 3 ; vi++){
                Vertex* vt = triangle->vertex(vi);
                int index = 0;
                for (; index < vertices.size(); index++){
                    if (vertices[index] == vt)
                        break;
                }
                if (vertices.size() == index) // New Vertex
                    vertices.push_back(vt);
                triangleIndex.push_back(index);
            }
            triangleIndicies.push_back(triangleIndex);
        }

        unsigned int triangleMeshLength = sizeof(unsigned short) + sizeof(unsigned int) + // Vertex : chunk_id and chunk_length
                                      sizeof(unsigned short) + // Vertices number
                                      sizeof(float) * 3 * vertices.size() + // Vertices List
                                      sizeof(unsigned short) + sizeof(unsigned int) + // Polygon : chunk_id and chunk_length
                                      sizeof(unsigned short) + // Polygons number
                                      sizeof(unsigned short) * 4 * triangleIndicies.size(); // Polygons List
        chunk_id = 0x4000;
        chunk_length = (unsigned short)this->triangles[groupI].first.size() + 1 +
                       sizeof(unsigned short) + sizeof(unsigned int) + // Triangle Mesh : chunk_id and chunk_length
                       triangleMeshLength;
        wholeLength += chunk_length + sizeof(unsigned short);

        fwrite(&chunk_id, 2, 1, pFile);
        fwrite(&chunk_length, 4, 1, pFile);

        const char* name = this->triangles[groupI].first.c_str();
        for (int i = 0 ; name[i] != 0 ; i++){
            fwrite(&name[i], sizeof(char), 1, pFile);
        }
        char temp = 0;
        fwrite(&temp, sizeof(char),1, pFile);

        // Triangle Mesh
        chunk_id = 0x4100;
        chunk_length = triangleMeshLength;
        fwrite(&chunk_id, 2, 1, pFile);
        fwrite(&chunk_length, 4, 1, pFile);

        // Vertices List
        chunk_id = 0x4110;
        chunk_length = sizeof(unsigned short) + sizeof(float) * 3 * vertices.size();
        fwrite(&chunk_id, 2, 1, pFile);
        fwrite(&chunk_length, 4, 1, pFile);
        unsigned short sizeOfVertices = (unsigned short)vertices.size();
        fwrite(&sizeOfVertices, sizeof(unsigned short), 1, pFile);
        for (int vi = 0 ; vi < vertices.size(); vi++){
            float x = (float)vertices[vi]->x();
            float y = (float)vertices[vi]->y();
            float z = (float)vertices[vi]->z();
            fwrite(&x, sizeof(float), 1, pFile);
            fwrite(&y, sizeof(float), 1, pFile);
            fwrite(&z, sizeof(float), 1, pFile);
        }

        // Triangle List
        chunk_id = 0x4120;
        chunk_length = sizeof(unsigned short) + // Polygons number
                       sizeof(unsigned short) * 4 * triangleIndicies.size();
        wholeLength += chunk_length;
        fwrite(&chunk_id, 2, 1, pFile);
        fwrite(&chunk_length, 4, 1, pFile);
        unsigned short sizeOfTriangles = (unsigned short)triangleIndicies.size();
        fwrite(&sizeOfTriangles, sizeof(unsigned short), 1, pFile);
        for (int tr = 0 ; tr < triangleIndicies.size(); tr++){
            for (int i = 0 ; i < 3 ; i++){
                fwrite(&triangleIndicies[tr][i], sizeof(unsigned short), 1, pFile);
            }
            unsigned short temp = 1;
            fwrite(&temp, sizeof(unsigned short), 1, pFile);
        }
    }
    /*
    fseek(pFile, sizeof(unsigned short) * 2 + sizeof(unsigned int), SEEK_SET);
    fwrite(&wholeLength, sizeof(unsigned short), 1, pFile);
    wholeLength += sizeof(unsigned short) + sizeof(unsigned int);
    fseek(pFile, sizeof(unsigned short) , SEEK_SET);
    fwrite(&wholeLength, sizeof(unsigned short), 1, pFile);
    */
    fclose(pFile);
    return;
}
