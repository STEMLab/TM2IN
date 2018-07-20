//
// Created by dongmin on 18. 1. 17.
//

#include "features/TriangleMeshGraph.h"
#include <iostream>

using namespace std;

bool TriangleMeshGraph::isNeighbor(ull id1, ull id2){
    for (ull i = 0 ; i < adjList[id1].size() ; i++){
        if (adjList[id1][i] == id2) return true;
    }
    return false;
}

int TriangleMeshGraph::makeAdjacentGraph() {
    vector<Triangle*>& triangleList = this->triangles;
    this->sizeOfTriangles = triangleList.size();
    adjList.assign(triangleList.size(), vector<ull>());
    for (ull i = 0 ; i < triangleList.size() - 1 ; i++){
        if (i % 10 == 0) printProcess(i, triangleList.size(), "make Graph");
        for (ull j = i + 1 ; j < triangleList.size() ; j ++){
            if (triangleList[i]->checkNeighbor(triangleList[j])){
                if (!triangleList[i]->isOpposite(triangleList[j])){
                    adjList[i].push_back(j);
                    adjList[j].push_back(i);
                }
                else{
                    cerr << "Opposite Triangle" << endl;
                    return -1;
                }
            }
        }
    }
    cout << endl;
    return 0;
}

bool TriangleMeshGraph::isClosedTriangleMesh(){
    for (ull i = 0; i < adjList.size() ; i++){
        if (adjList[i].size() != 3){
            return false;
        }
    }
    return true;
}

vector<ull> TriangleMeshGraph::bfs(ull src, vector<bool>& checked){
    vector<ull> component;
    queue<int> wait_queue;
    wait_queue.push(src);
    while (wait_queue.size() > 0){
        int current = wait_queue.front();
        wait_queue.pop();
        component.push_back(current);

        for (unsigned int nb = 0 ; nb < adjList[current].size() ; nb++){
            ull next_surface = adjList[current][nb];
            if (checked[next_surface]) continue;
            else{
                checked[next_surface] = true;
                wait_queue.push(next_surface);
            }
        }
    }
    return component;
}

vector<vector<ull>> TriangleMeshGraph::getConnectedComponent() {
    vector<vector<ull>> connectedComponent;
    vector<bool> checked(this->sizeOfTriangles, false);

    for (ull i = 0 ; i < this->sizeOfTriangles; i++){
        if (!checked[i]){
            checked[i] = true;
            vector<ull> oneComponent;
            oneComponent = this->bfs(i, checked);
            connectedComponent.push_back(oneComponent);
        }
    }
    return connectedComponent;

}

vector<ull> TriangleMeshGraph::getNeighbors(ull i) {
    return adjList[i];
}

bool TriangleMeshGraph::isEmpty() {

    return (sizeOfTriangles == 0);
}
