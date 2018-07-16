
#include "fileio/JSONMaker.h"

#include <fstream>

int JSONMaker:: printJSON(ofstream& fout, vector<Solid*>& ts){
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

int JSONMaker::printJSON(ofstream& fout, Solid* space){
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
        //if (cp[id]->getVerticesSize() == 1) cout << "1 tri" << endl;

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

int JSONMaker::printTriangleJSON(ofstream& fout, vector<Solid *>& spaces) {
    fout << "{ \n";
    fout << " \"spaces\" : [ \n";
    for (unsigned int index = 0 ; index < spaces.size(); index++){
        if (JSONMaker::printTriangleJSON(fout, spaces[index])){
            cout << "This space doesn't have Triangulation" << endl;
        }
        else{
            if (index != spaces.size() - 1 ) fout << ", \n";
            else fout <<"\n";
        }
    }
    fout << "]" << endl;
    fout << "}" << endl;
    return 0;
}


int JSONMaker::printTriangleJSON(ofstream &fout, Solid *space) {
    fout << "{\n";
    fout<< " \"name\" : \"" << space->name << "\", \n" ;
    JSONMaker::printTriangleJSON(fout, space->surfacesList);
    fout <<"}";
    return 0;
}

int JSONMaker::printTriangleJSON(ofstream &fout, std::vector<Surface *> &surfacesList) {
    fout << " \"Surfaces\" : [ \n";
    for (unsigned int id = 0 ; id < surfacesList.size() ; id++)
    {
        string triangles = surfacesList[id]->toJSONWithTriangles();
        fout << triangles;

        if (id != surfacesList.size() - 1)
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
