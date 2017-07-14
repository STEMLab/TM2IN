#include "ManagerImpl.h"


#include "JSONMaker.h"

#include <fstream>

ManagerImpl::ManagerImpl()
{
    //ctor
}

ManagerImpl::~ManagerImpl()
{
    //dtor
}

int ManagerImpl::import(char* file_path){
    this->objcl = di->import(file_path, this->check);
    if (this->objcl == NULL) return -1;
    else return 0;
}

void ManagerImpl::makeSurfaces(){
    objcl->makeSurfaces(this->check);
}


void ManagerImpl::cleaning(){
    objcl->cleaning(check);
    cout << "Removing..." << endl;
    objcl->removeVertexList();

    //TODO : combine CombinedPolygons
}

int ManagerImpl::exportJSON(string f_path)
{
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
