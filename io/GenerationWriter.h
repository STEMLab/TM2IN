#ifndef GenerationWriter_H_INCLUDED
#define GenerationWriter_H_INCLUDED

#include "features/PolyhedralSurface.h"
#include "util.h"
#include <fstream>

using namespace std;
using namespace TM2IN;

class GenerationWriter{
public:
    string process_path;
    PolyhedralSurface* space;

    GenerationWriter(string pp){
        process_path = pp;
    }

    void start(PolyhedralSurface* _space);
    void write();
private:

    void writeJSON();
    void writeStat();
};

#endif // GenerationWriter_H_INCLUDED
