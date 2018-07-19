#ifndef JSONMAKER_H_INCLUDED
#define JSONMAKER_H_INCLUDED

#include <vector>
#include <string>

#include "features/Surface.h"
#include "features/PolyhedralSurface.h"

using namespace std;

namespace TM2IN{
    namespace io {
        int exportJSON(string output_file, vector<PolyhedralSurface *> &ts, bool triangulation_out=false);
    }
};

#endif // JSONMAKER_H_INCLUDED
