#include "features/TriangleMeshGraph.h"
#include <iostream>

using namespace std;

bool TriangleMeshGraph::isNeighbor(ull id1, ull id2){
    for (ull i = 0 ; i < adjList[id1].size() ; i++){
        if (adjList[id1][i] == id2) return true;
    }
    return false;
}

void TriangleMeshGraph::makeAdjacentGraph(TriangleMesh* tm){
    vector<Triangle> tri_list(tm->triangles[0].second);
    adjList.assign(tri_list.size(), vector<ull>());
    for (ull i = 0 ; i < tri_list.size() - 1 ; i++){
        printProcess(i, tri_list.size(), "makeGraph");
        for (ull j = i + 1 ; j < tri_list.size() ; j ++){
            if (tri_list[i].isAdjacent(tri_list[j])){
                if (tri_list[i].isOpposite(tri_list[j])){
                    cout << "\nOpposite" << endl;
                }
                else{
                    adjList[i].push_back(j);
                    adjList[j].push_back(i);
                }
            }
        }
    }
}

bool TriangleMeshGraph::isClosedTriangleMesh(){
    for (ull i = 0; i < adjList.size() ; i++){
        if (adjList[i].size() != 3){
            return false;
        }
    }
    return true;
}

void TriangleMeshGraph::print_bfs(){
    vector<bool> checked(this->size, false);

    for (unsigned int i = 0 ; i < this->size ; i ++){
        if (checked[i]) continue;
        queue<int> wait_queue;
        wait_queue.push(i);
        checked[i] = true;

        int level = 0;
        cout << i << endl;

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
