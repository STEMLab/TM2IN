
#include "fileio/JSONMaker.h"

#include <fstream>

int JSONMaker:: printJSON(ofstream& fout, vector<Space*>& ts){
    fout << "{ \n";
    fout << " \"spaces\" : [ \n";
    for (unsigned int index = 0 ; index < ts.size(); index++){
        JSONMaker::printJSON(fout, ts[index]);
        if (index != ts.size() - 1 ) fout << ", \n";
        else fout <<"\n";
    }
    fout << "]" << endl;
    fout << "}" << endl;
    return 0;
}

int JSONMaker::printJSON(ofstream& fout, Space* space){
    fout << "{\n";
    fout<< " \"name\" : \"" << space->name << "\", \n" ;

    printJSON(fout, space->surfacesList);
    fout <<"}";
    return 0;
}

int JSONMaker::printJSON(ofstream& fout, vector<Surface*>& cp){
    cout << "\nThe number of Surfaces : " << cp.size() <<endl;
    fout << " \"Surfaces\" : [ \n";
    for (unsigned int id = 0 ; id < cp.size() ; id++)
    {
        //if (cp[id]->v_list.size() == 1) cout << "1 tri" << endl;

        string cp_coords = cp[id]->toJSONString();
        fout << cp_coords;

        if (id != cp.size() - 1)
        {
            fout << ", \n";
        }
        else
        {
            fout <<" \n";
        }
    }
    fout << "] \n";
    return 0;
}
