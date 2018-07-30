#ifndef JSONMAKER_H_INCLUDED
#define JSONMAKER_H_INCLUDED

#include <vector>
#include <string>

#include "features/Surface.h"

using namespace std;

namespace TM2IN{
    namespace io {
        int exportRoomBoundaryJSON(string output_file, vector<Room *> &rooms, int boundary_mode=0);
    }
};

#endif // JSONMAKER_H_INCLUDED
