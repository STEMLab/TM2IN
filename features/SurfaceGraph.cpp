#include "features/SurfaceGraph.h"
#include <iostream>
#include <compute/Surface_pair_computation.h>

bool SurfaceGraph::isNeighbor(ull id1, ull id2){
    for (ull i = 0 ; i < adjList[id1].size() ; i++){
        if (adjList[id1][i] == id2) return true;
    }
    return false;
}

void SurfaceGraph::makeAdjacentGraph(vector<Surface*>& surface_list){
    adjList.assign(surface_list.size(), vector<ull>());
    for (ull i = 0 ; i < surface_list.size() - 1 ; i++){
        Surface* surface_i = surface_list[i];
        for (ull j = i + 1 ; j < surface_list.size() ; j ++){
            Surface* surface_j = surface_list[j];
            int shareEdgeCount = SurfacePairComputation::doShareEdge(surface_i, surface_j);
            if (shareEdgeCount){
                adjList[i].push_back(j);
                adjList[j].push_back(i);
            }

            // test
            if (shareEdgeCount > 1){
                cerr << "They have two share?" << endl;
            }
        }
    }
}


bool SurfaceGraph::isClosedSurface(){
    vector<bool> checked(this->adjList.size(), false);

    queue<int> wait_queue;
    wait_queue.push(0);
    checked[0] = true;

    int surfaceCount = 0;

    while (wait_queue.size() > 0){
        int current = wait_queue.front();
        wait_queue.pop();

        surfaceCount += 1;

        for (unsigned int nb = 0 ; nb < adjList[current].size() ; nb++){
            ull next_surface = adjList[current][nb];
            if (checked[next_surface]) continue;
            else{
                checked[next_surface] = true;
                wait_queue.push(next_surface);
            }
        }
    }

    if (surfaceCount != this->adjList.size()){
        cout << surfaceCount << endl;
        return false;
    } else
        return true;

}