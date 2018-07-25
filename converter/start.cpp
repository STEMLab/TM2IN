//
// Created by dongmin on 18. 7. 18.
//


#include "converter/Converter.h"

#include "features/TriangleMesh.h"
#include "io/tvr.h"
#include "io/collada.h"
#include "io/max3ds.h"

int Converter::importData() {
    switch(options->input_type){
        case 1:
            this->rooms = TM2IN::io::importTVR((options->input_dir + options->input_file).c_str());
            break;
        case 2:
            this->rooms = TM2IN::io::import3DS((options->input_dir + options->input_file).c_str());
            break;
        case 3:
            this->rooms = TM2IN::io::importDAE((options->input_dir + options->input_file).c_str());
            break;
        default:
            throw std::runtime_error("\n\nImport Mesh has some problem\n\n");
    }
    if (this->rooms.size() == 0){
        throw std::runtime_error("\n\nImport Mesh has some problem\n\n");
    }
    return 0;
}

int Converter::partitionTriangleMeshByComponent() {
    int i = 0;
    vector<TriangleMesh*> new_mesh_list;
    while ( i < this->mesh_list.size() ){
        assert(this->mesh_list[i]->checkClosed());

        int result = this->mesh_list[i]->partitionByComponent(new_mesh_list);
        if(result == -1) return -1;
        else
            i++;
    }
    this->mesh_list = new_mesh_list;
    cout << "The Number of Mesh : " << this->mesh_list.size() << endl;

    return 0;
}

int Converter::initTriangleMesh() {
    clock_t begin = clock();
    for (int i = 0 ; i < this->mesh_list.size() ; i++){
        this->mesh_list[i]->init();
    }
    clock_t end = clock();

    cout << "make graph time : " << double(end - begin) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}

int Converter::remainSelectedMesh(int arch) {


    printf("There are %lu Remaining Meshes.\n\n", this->mesh_list.size());
    return 0;
}

int Converter::convertTriangleMeshToSpace() {
    for (int space_id = 0 ; space_id < this->mesh_list.size() ; space_id++){
        PolyhedralSurface* space = new PolyhedralSurface();
        space->setName(this->mesh_list[space_id]->name);
        if (space->convertTrianglesToSurfaces(this->mesh_list[space_id]->triangles)){
            cout << "make Surfaces error" << endl;
            return -1;
        }
        space->vertices = this->mesh_list[space_id]->vertices;
        this->spaceList.push_back(space);
    }
    this->mesh_list.clear();
    return 0;
}