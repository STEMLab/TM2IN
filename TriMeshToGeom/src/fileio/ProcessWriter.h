#ifndef PROCESSWRITER_H_INCLUDED
#define PROCESSWRITER_H_INCLUDED

#include "data/Space.h"
#include "logic/util.h"
#include <fstream>

using namespace std;

class ProcessWriter{
public:
    string process_path;
    string data_name;

    ProcessWriter(string pp, string dn){
        process_path = pp;
        data_name = dn;

    }

    void writeBeforeJoin(ull num, int space_num);
    void writeRoughSurfaces(ull num, int space_num);
    void writeGenerationJSON(int gen, vector<Space*>& space_list);
    void writeGenerationStat(int gen, ull num, int space_num);
};

#endif // PROCESSWRITER_H_INCLUDED
