#ifndef GenerationWriter_H_INCLUDED
#define GenerationWriter_H_INCLUDED

#include "data/Space.h"
#include "logic/util.h"
#include <fstream>

using namespace std;

class GenerationWriter{
public:
    string process_path;

    GenerationWriter(string pp){
        process_path = pp;
    }

    void writeGenerationJSON(int gen, vector<Space*>& space_list);
    void writeGenerationStat(int gen, ull num);
    void clearGenerationStat();
};

#endif // GenerationWriter_H_INCLUDED
