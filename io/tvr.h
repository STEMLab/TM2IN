//
// Created by dongmin on 18. 7. 16.
//

#ifndef TM2IN_TVR_H
#define TM2IN_TVR_H

#include "features/IndoorComponent.h"

namespace TM2IN{
    namespace io{
        /**
         * @ingroup public_api
         * @brief Imports TVR file and returns a vector of Room.
         */
        std::vector<Room*> importTVR(const char *filePath);

    }
}


#endif //TM2IN_TVR_H
