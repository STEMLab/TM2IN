#ifndef GenerationWriter_H_INCLUDED
#define GenerationWriter_H_INCLUDED

#include "features/Room.h"
#include "util.h"
#include <fstream>

using namespace std;
using namespace TM2IN;

/**
 * @ingroup unused
 * @todo
 */
class GenerationWriter{
public:
    string process_path;
    Room* space;

    GenerationWriter(string pp){
        process_path = pp;
    }

    void start(Room* _space);
    void write();
private:

    void writeJSON();
    void writeStat();
};

#endif // GenerationWriter_H_INCLUDED
