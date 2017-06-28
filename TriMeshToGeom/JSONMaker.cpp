
#include "JSONMaker.h"

#include <fstream>

int JSONMaker::printJSON(vector<pair<string, vector<CombinedPolygon*>>>& cp, string& filename){
    ofstream fout;
    fout.open(filename, ios::out|ios::trunc);

    if (!fout) return -1;

    fout << "{" <<endl;
    fout << "\"objects\" : [" << endl;
    for (unsigned int index = 0 ; index < cp.size(); index++){
        fout << "{\n \"name\" : \"" << cp[index].first << "\"," << endl;
        fout << " \"polygons\" : [";
        for (unsigned int cp_id = 0 ; cp_id < cp[index].second.size() ; cp_id++){
            string cp_coords = cp[index].second[cp_id]->toString();
            fout << cp_coords;
        }
        fout << "] }";
        if (index != cp.size() - 1 ) fout << "," << endl;
        else fout << endl;

    }
    fout << "]" << endl;
    fout << "}" << endl;
    return 0;
}
