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
#include <compute/Connect_halfedges.h>
#include <cgal/Features_to_CGAL_object.h>
#include "features/HalfEdge.h"
#include <time.h>
#include <random>

void TriangleMesh::init() {
    // this->resolveWrongTriangle();
    for (int i = 0 ; i < this->triangles.size() ; i++){
        for (HalfEdge* he : this->triangles[i]->boundaryEdges){
            assert(he->getOppositeEdge() == NULL);
        }
    }
    CGAL_User::connectOppositeHalfEdges(this->triangles);
    this->makeGraph();
}

void TriangleMesh::makeGraph(){
    this->graph = new TriangleMeshGraph(this->triangles);
    this->graph->makeAdjacentGraph();
}

bool TriangleMesh::checkClosed() {
    if (this->graph == NULL || this->graph->isEmpty()){
        cerr << "You didn't make graph." << endl;
        assert(false);
    }
    else
        return this->graph->isClosedTriangleMesh();
}

int TriangleMesh::partitionByComponent(vector<TriangleMesh *> &new_mesh_list) {
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

bool TriangleMesh::isFurniture() {
    vector<Triangle_3> cgal_triangles;
    for (int i = 0 ; i < this->triangles.size() ; i++){
        cgal_triangles.push_back(CGAL_User::getCGALTriangle(this->triangles[i]));
    }

    int innerCount = 0, outerCount = 0;
    Bbox_3 bbox3 = CGAL::bbox_3(cgal_triangles.begin(), cgal_triangles.end());

    int index = 0;
    std::random_device rd;
    std::mt19937 gen(rd());

    while (innerCount < 10 && outerCount < 10){
        double coords[3];
        double outsideValue;
        std::uniform_real_distribution<> dis( 0, bbox3.max(index%3) - bbox3.min(index % 3));
        if (index % 6 >= 3){
            outsideValue = bbox3.max(index % 3) + dis(gen);
        }
        else{
            outsideValue = bbox3.min(index % 3) - dis(gen);
        }
        coords[index % 3] = outsideValue;
        int next1 = (index + 1) % 3;
        int next2 = (index + 2) % 3;
        std::uniform_real_distribution<> dis1 (bbox3.min(next1), bbox3.max(next1));
        std::uniform_real_distribution<> dis2 (bbox3.min(next2), bbox3.max(next2));

        coords[next1] = dis1(gen);
        coords[next2] = dis2(gen);

        Point_3 outsidePoint(coords[0], coords[1], coords[2]);
        assert(outsidePoint[0] > bbox3.xmax() || outsidePoint[0] < bbox3.xmin() ||
                       outsidePoint[1] > bbox3.ymax() || outsidePoint[1] < bbox3.ymin() ||
                       outsidePoint[2] > bbox3.zmax() || outsidePoint[2] < bbox3.zmin());

        double minDist = INT_MAX;
        int minIndex = -1;
        for (int i = 0 ; i < cgal_triangles.size() ; i++){
            Triangle_3& cgal_triangle = cgal_triangles[i];
            double dist = CGAL::squared_distance(cgal_triangle, outsidePoint);
            if (minDist > dist){
                minDist = dist;
                minIndex = i;
            }
        }

        Vector_3 normal_of_triangle = cgal_triangles[minIndex].supporting_plane().orthogonal_vector();
        double vector_coords[3] = {0, 0, 0};
        vector_coords[index % 3] = index % 6 >= 3? -1 : 1;
        Vector_3 vector_to_min_plane(vector_coords[0], vector_coords[1], vector_coords[2]);

        double angle = CGALCalculation::getAngle(normal_of_triangle, vector_to_min_plane);
        assert(angle != 90.0);
        if (angle > 90 && angle <= 180){
            //direction to outside
            outerCount++;
        }
        else if (angle < 90 && angle >= 0){
            //direction to inside
            innerCount++;
        }
        else{
            assert(0);
        }
    }
    if (innerCount > 0 && outerCount > 0){
        cerr << "\nSomething wrong in is Furniture" << endl;
        cerr << "innerCount : " << innerCount << endl;
        cerr << "outerCount : " << outerCount << "\n" << endl;
    }
    if (outerCount > innerCount){
        return true;
    }
    else if (innerCount > outerCount)
        return false;
}
