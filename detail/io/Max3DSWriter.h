//
// Created by dongmin on 18. 7. 19.
//

#ifndef TM2IN_MAX3DSWRITER_H
#define TM2IN_MAX3DSWRITER_H

#include <cstdio>
#include "features/IndoorComponent.h"

namespace TM2IN {
    namespace detail {
        namespace io {
            /**
             * @ingroup imp_details
             */
            class Max3DSWriter {
            public:
                Max3DSWriter(FILE* pFile);
                int write(std::vector<Room*> rooms);
            private:
                FILE* pFile;
            };
        }
    }
}

#endif //TM2IN_MAX3DSWRITER_H
