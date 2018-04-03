#include "features/SurfaceGraph.h"
#include <iostream>
#include <compute/SurfacePairComputation.h>

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
            if (SurfacePairComputation::doShareEdge(surface_i, surface_j)){
                adjList[i].push_back(j);
                adjList[j].push_back(i);
            }
        }
    }
}


bool SurfaceGraph::isClosedTriangleMesh(){
    for (ull i = 0; i < adjList.size() ; i++){
        if (adjList[i].size() != 3){
            return false;
        }
    }
    return true;
}

void SurfaceGraph::print_bfs(){
    vector<bool> checked(this->adjList.size(), false);

    for (unsigned int i = 0 ; i < this->adjList.size() ; i ++){
        if (checked[i]) continue;
        queue<int> wait_queue;
        wait_queue.push(i);
        checked[i] = true;

        int level = 0;
        cout << "from " <<i << "'s graph" << endl;

        while (wait_queue.size() > 0){
            int current = wait_queue.front();
            wait_queue.pop();

            level += 1;

            for (unsigned int nb = 0 ; nb < adjList[current].size() ; nb++){
                ull next_surface = adjList[current][nb];
                if (checked[next_surface]) continue;
                else{
                    checked[next_surface] = true;
                    cout << next_surface << " ";
                    wait_queue.push(next_surface);
                }
            }
        }

        cout << "------------\n" << endl;
    }

}
