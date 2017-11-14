#include "manage/FlatRoomMaker.h"

#include "fileio/JSONMaker.h"
#include "fileio/CombinedIO.h"
#include <fstream>

using namespace std;

FlatRoomMaker::FlatRoomMaker()
{
    //ctor
}

FlatRoomMaker::~FlatRoomMaker()
{
    //dtor
}

int FlatRoomMaker::import(const char* file_path){
    this->objcl = di->import(file_path, this->check);
    if (this->objcl == NULL) return -1;
    else return 0;
}

int FlatRoomMaker::makeSurfaces(double degree){
    return objcl->makeSurfaces(degree);
}


int FlatRoomMaker::cleaning(int max_gener, double startDegree){
    objcl->process(check, max_gener, startDegree);
    //if (objcl->combineSurfaces(check, max_gener, startDegree) == -1) return -1;
    return 0;
}

int FlatRoomMaker::makeSolids(double degree){
    //if (objcl -> makeWall(degree)) return -1;
    //if (objcl -> makeFloorAndCeiling()) return -1;
    if (objcl -> makeSolid() ) return -1;
    return 0;
}

int FlatRoomMaker::exportJSON(string f_path)
{
    cout << "export JSON" << endl;
    ofstream fout;
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printJSON(fout, this->objcl->space_list))
    {
        return -1;
    }
    fout.close();
    return 0;
}

int FlatRoomMaker::exportCombined(string f_path)
{
    ofstream fout;
    fout.open(f_path, ios::out|ios::trunc|ios::binary);
    if (!fout) return -1;
    if (CombinedIO::exportBinary(fout, this->objcl->space_list))
    {
        return -1;
    }
    fout.close();
    return 0;
}

int FlatRoomMaker::importCombined(string f_path)
{
    ifstream fin;
    fin.open(f_path, ios::in|ios::binary);
    if (!fin){
        cout << "No file" << endl;
        return -1;
    }
    if (CombinedIO::importBinary(fin, this->objcl)) return 1;
    fin.close();
    return 0;
}

