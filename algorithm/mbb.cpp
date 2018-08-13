//
// Created by dongmin on 18. 7. 24.
//

#include "features/Wall/Surface.h"
#include "mbb.h"

using namespace std;

namespace TM2IN{
    namespace algorithm{
        CGAL::Bbox_3 getMBB(std::vector<Wall::Surface *> &surfacesList){
            vector<vector<double>> v_minmax;
            v_minmax.assign(2, vector<double>());

            for (int i = 0 ; i < 3 ; i++)
            {
                v_minmax[1].push_back(-100000.0000);
                v_minmax[0].push_back(100000.0000);
            }

            for (ull i = 0 ; i < surfacesList.size() ; i++){
                Wall::Surface* cp = surfacesList[i];
                cp->updateMBB();
                for (int j = 0 ; j < 3 ; j++){
                    v_minmax[1][j] = max(v_minmax[1][j], cp->getMBB()->max(j));
                    v_minmax[0][j] = min(v_minmax[0][j], cp->getMBB()->min(j));
                }
            }

            CGAL::Bbox_3 bbox_3(v_minmax[0][0],v_minmax[0][1], v_minmax[0][2], v_minmax[1][0], v_minmax[1][1], v_minmax[1][2]);
            return bbox_3;
        }


    }
}