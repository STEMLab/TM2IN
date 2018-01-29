#include <fileio/JSONMaker.h>
#include "manage/RoomMaker.h"


int RoomMaker::importMesh(const char* file_path){
    this->mesh = di->import(file_path);
    if (this->mesh == NULL) return -1;
    else return 0;
}

int RoomMaker::convertTriangleMeshToSpace(double degree){
    for (int space_id = 0 ; space_id < this->mesh->triangles.size() ; space_id++){
        Space* space = new Space();
        space->setName(this->mesh->triangles[space_id].first);
        if (space->convertTrianglesToSurfaces(this->mesh->triangles[space_id].second)){
            cout << "make Surfaces error" << endl;
            return -1;
        }
        this->spaceList.push_back(space);
    }
    this->vertices = this->mesh->vertices;
    return 0;
}

int RoomMaker::exportSpaceJSON(string f_path)
{
    ofstream fout;
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printJSON(fout, this->spaceList)) return -1;
    fout.close();
    return 0;
}

int RoomMaker::exportTriangulationJSON(string f_path) {
    ofstream fout;
    fout.open(f_path, ios::out|ios::trunc);

    if (!fout) return -1;
    if (JSONMaker::printTriangleJSON(fout, this->spaceList)) return -1;
    fout.close();
    return 0;
}


/*
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
*/
