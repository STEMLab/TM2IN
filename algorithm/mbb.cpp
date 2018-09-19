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
                    v_minmax[1][j] = max(v_minmax[1][j], bbox3_max_with_index(cp->getMBB(), j));
                    v_minmax[0][j] = min(v_minmax[0][j], bbox3_min_with_index(cp->getMBB(), j));
                }
            }

            CGAL::Bbox_3 bbox_3(v_minmax[0][0],v_minmax[0][1], v_minmax[0][2], v_minmax[1][0], v_minmax[1][1], v_minmax[1][2]);
            return bbox_3;
        }

        double bbox3_max_with_index(CGAL::Bbox_3 bbox3, int index) {
            if (index == 0)
                return bbox3.xmax();
            else if (index == 1)
                return bbox3.ymax();
            else if (index == 2)
                return bbox3.zmax();
            throw std::runtime_error("MinimumBoundingBox::max(int index) : out of range");
        }

        double bbox3_min_with_index(CGAL::Bbox_3 bbox3, int index) {
            if (index == 0)
                return bbox3.xmin();
            else if (index == 1)
                return bbox3.ymin();
            else if (index == 2)
                return bbox3.zmin();
            throw std::runtime_error("bbox3_min_with_index(int index) : out of range");
        }


    }
}