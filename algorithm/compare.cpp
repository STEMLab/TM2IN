//
// Created by dongmin on 18. 7. 24.
//

#include <predefine.h>
#include "compare.h"

namespace TM2IN{
    namespace algorithm{
        bool greater(Vertex * a, Vertex * b) {
            ll multiple = pow(10, DECIMAL);
            int ax = (int)(a->x() * multiple);
            int bx = (int)(b->x() * multiple);
            int ay = (int)(a->y() * multiple);
            int by = (int)(b->y() * multiple);
            int az = (int)(a->z() * multiple);
            int bz = (int)(b->z() * multiple);

            if (ax < bx){
                return true;
            }
            else if (ax > bx){
                return false;
            }
            else{
                if (ay < by)
                {
                    return true;
                }
                else if (ay > by)
                {
                    return false;
                }
                else{
                    if (az < bz)
                    {
                        return true;
                    }
                    else if (az > bz)
                        return false;
                    else
                        return false;
                }
            }
        }
    }
}