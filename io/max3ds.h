//
// Created by dongmin on 18. 7. 16.
//

#ifndef TM2IN_MAX3DS_H
#define TM2IN_MAX3DS_H

#include "features/Room.h"

namespace TM2IN{
    namespace io{
        std::vector<Room*> import3DS(const char *filePath);
        int export3DS(const char *filePath, std::vector<Room*> rooms);
    }
}



#endif //TM2IN_MAX3DS_H
