#include <fileio/JSONMaker.h>
#include "manage/RoomMaker.h"


int RoomMaker::importMesh() {
    string filePath = paths["resource"] + paths["filename"] + "." + paths["filetype"];
    this->mesh = di->import(filePath.c_str());
    if (this->mesh == NULL) return -1;
    else return 0;
}

int RoomMaker::convertTriangleMeshToSpace() {
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

int RoomMaker::convertSpaceToTriangleMesh(){

}

int RoomMaker::exportSpace(const char* fileName) {
    if (de->exportSpace(this->spaceList, fileName)) return 1;
    return 0;
}
