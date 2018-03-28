#include <fileio/JSONMaker.h>
#include "manage/RoomMaker.h"


int RoomMaker::importMesh() {
    string filePath = paths["resourceDir"] + paths["filename"] + "." + paths["filetype"];
    this->mesh = di->import(filePath.c_str());
    if (this->mesh == NULL) return -1;
    if (this->mesh->triangles.size() == 0) return -1;
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
    this->mesh->triangles.clear();
    this->mesh->vertices.clear();
    for (int spaceID = 0 ; spaceID < this->spaceList.size() ; spaceID++){
        Space* space = this->spaceList[spaceID];
        vector<Triangle*> triangleList = space->getTriangleListOfAllSurfaces();
        this->mesh->triangles.push_back(make_pair(space->name, triangleList));
    }
    this->mesh->vertices = this->vertices;
    return 0;
}

int RoomMaker::exportSpace() {
    string filePath = paths["versionDir"] + "surfaces.json";
    if (de->exportSpace(this->spaceList, filePath.c_str())) return 1;
    return 0;
}

void RoomMaker::setPaths(map<string, string> _paths) {
    this->paths = _paths;
}
