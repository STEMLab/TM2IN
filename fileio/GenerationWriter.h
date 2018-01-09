#ifndef GenerationWriter_H_INCLUDED
#define GenerationWriter_H_INCLUDED

#include "features/Space.h"
#include "util.h"
#include <fstream>

using namespace std;

class GenerationWriter{
public:
    string process_path;
    Space* space;
    int generation = 0;

    GenerationWriter(string pp){
        process_path = pp;
    }

    void start(Space* _space);
    void write();
private:

    void writeJSON();
    void writeStat();
};

#endif // GenerationWriter_H_INCLUDED
