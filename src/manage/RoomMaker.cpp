#include "RoomMaker.h"

#include "fileio/CombinedIO.h"

int RoomMaker::import(const char* file_path){
    this->objcl = di->import(file_path, this->check);
    if (this->objcl == NULL) return -1;
    else return 0;
}



int RoomMaker::exportSpaceJSON(string f_path)
{
    ofstream fout;
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printJSON(fout, this->objcl->space_list)) return -1;
    fout.close();
    return 0;
}

int RoomMaker::exportSimpleSpaceJSON(string f_path)
{
    ofstream fout;
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printJSON(fout, this->objcl->simple_space_list)) return -1;
    fout.close();
    return 0;
}

int RoomMaker::exportCombined(string f_path)
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

int RoomMaker::importGeneration(string f_path)
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
