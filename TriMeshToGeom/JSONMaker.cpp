
#include "JSONMaker.h"

#include <fstream>

int JSONMaker::printJSON(vector<pair<string, vector<CombinedPolygon*>>>& cp, string& filename){
    ofstream fout;
    fout.open(filename);

    if (!fout) return -1;

    fout << "{" <<endl;
    fout << "\"objects\" : [" << endl;
    for (int index = 0 ; index < cp.size(); index++){
        fout << "{\n \"name\" : \"" << cp[index].first << "\"," << endl;
        fout << " \"polygons\" : [";
        for (int cp_id = 0 ; cp_id < cp[index].second.size() ; cp_id++){
            fout << cp[index].second[cp_id]->toString();
        }
        fout << "] }";
        if (index != cp.size() - 1 ) fout << "," << endl;
        else fout << endl;
    }
    fout << "]" << endl;
    fout << "}" << endl;
}
