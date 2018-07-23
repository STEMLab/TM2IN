//
// Created by dongmin on 18. 1. 17.
//

#include "features/TriangleMesh.h"
#include <utility>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <logic/check.h>
#include <compute/Connect_halfedges.h>
#include <time.h>
#include <random>
#include "features/HalfEdge.h"
#include "features/Triangle.h"
#include "features/TriangleMeshGraph.h"

void TriangleMesh::init() {
    for (int i = 0 ; i < this->triangles.size() ; i++){
        for (HalfEdge* he : this->triangles[i]->getExteriorBoundary()){
            assert(he->getOppositeEdge() == NULL);
        }
    }
    TMIC::connectOppositeHalfEdges(this->triangles);
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

void TriangleMesh::clear(){
    triangles.clear();
    vertices.clear();
    graph->clear();
}



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
        cgal_triangles.push_back(this->triangles[i]->CGAL_triangle());
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
            throw std::runtime_error("is Furniture Error");
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
