//
// Created by dongmin on 18. 7. 16.
//

#ifndef TM2IN_TVR_H
#define TM2IN_TVR_H

#include <features/TriangleMesh.h>

namespace TM2IN{
    namespace io{
        vector<Room*> importTVR(const char *filePath);
    }
}


#endif //TM2IN_TVR_H
