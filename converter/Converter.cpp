#include <compute/SurfacesListComputation.h>
#include "TriangulationConverter.h"
#include "Converter.h"

#include "features/HalfEdge.h"
#include "fileio/JSONMaker.h"
#include "fileio/export/MeshExporter.h"

int Converter::importMesh() {
    string filePath = paths["resourceDir"] + paths["filename"] + "." + paths["filetype"];
    this->mesh_list = di->import(filePath.c_str());
    cout << "Whole Facet number is " << SurfacesListComputation::countTriangles(this->mesh_list) << endl;
    if (this->mesh_list.size() == 0) return -1;
    else return 0;
}

int Converter::convertTriangleMeshToSpace() {
    for (int space_id = 0 ; space_id < this->mesh_list.size() ; space_id++){
        Space* space = new Space();
        space->setName(this->mesh_list[space_id]->name);
        if (space->convertTrianglesToSurfaces(this->mesh_list[space_id]->triangles)){
            cout << "make Surfaces error" << endl;
            return -1;
        }
        space->vertices = this->mesh_list[space_id]->vertices;
        this->spaceList.push_back(space);
        break;
    }
    this->mesh_list.clear();
    return 0;
}

int Converter::convertSpaceToTriangleMesh(){
    this->mesh_list.clear();
    for (int spaceID = 0 ; spaceID < this->spaceList.size() ; spaceID++){
        Space* space = this->spaceList[spaceID];
        vector<Triangle*> triangleList = space->getTriangleListOfAllSurfaces();
        for (Triangle* triangle : triangleList){
            vector<HalfEdge*> edges = triangle->getBoundaryEdgesList();
            for (HalfEdge* he : edges){
                assert(he->getOppositeEdge() == NULL);
            }
        }
        TriangleMesh* mesh = new TriangleMesh();
        mesh->triangles = triangleList;
        mesh->vertices = space->vertices;
        mesh->name = space->name;
        this->mesh_list.push_back(mesh);
    }
    return 0;
}

int Converter::exportSpace() {
    string filePath = paths["versionDir"] + paths["fileName"];
    if (de->exportSpace(this->spaceList, filePath.c_str())) return 1;
    return 0;
}

void Converter::setPaths(map<string, string> _paths) {
    this->paths = _paths;
    this->paths["fileName"] = "surfaces.json";
}

void Converter::tagID() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        Space *space = this->spaceList[it];
        space->tagID();
    }
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

int Converter::remainStructure() {
    int i = 0;
    while (i < this->mesh_list.size()){
        if (this->mesh_list[i]->isFurniture()){
            this->mesh_list.erase(this->mesh_list.begin() + i);
        } else
            i++;
    }
    printf("There are %lu Remaining Meshes.\n\n", this->mesh_list.size());
    return 0;
}

int Converter::mergeSurfaces() {

    for (ull it = 0 ; it < this->spaceList.size(); it++)
    {
        Checker::merge_degree = 10.0;
        Space* space = this->spaceList[it];
        if (this->generation_writer) this->generation_writer->start(space);
        space->generation++;

        // check duplicate coordinates
        if (this->spaceList[it]->checkDuplicateVertexInSurfaces()) return -1;

        // limit degree of same normal vector angle

        if (processGenerations(space)) return -1;

        if (space->checkSurfaceValid() == -1){ cout << "Surface is not valid" << endl; return -1; }
        space->sortSurfacesByArea();
    }
    return 0;
}

int Converter::processGenerations(Space *space) {
    ll p_size = (ull)space->surfacesList.size();
    while (true){
        assert(p_size > 0);
        cout << "generation " << space->generation << ": " << space->surfacesList.size()<< endl;
        if (space->combineSurface() == -1){
            cerr << "combine error" << endl;
            return -1;
        }

        if (space->checkSurfaceValid() == -1){
            cerr << "Surface is not valid" << endl;
            return -1;
        }

        if (p_size == (int)space->surfacesList.size()) {
            cout << "generation " << space->generation  << " done..\n\n\n"<< endl;
            break;
        }
        else p_size = (int)space->surfacesList.size();

        if (this->generation_writer) this->generation_writer->write();
        if (Checker::merge_degree < 40) Checker::merge_degree += 5.0;

        space->generation++;
    }
    return 0;
}

int Converter::checkSelfIntersection() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        Space *space = this->spaceList[it];
        space->checkSelfIntersection();
    }
    return 0;
}

int Converter::simplifyShareEdge() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        Space *space = this->spaceList[it];
        cout << "simplify space " << space->name << endl;
        space->simplifySegment();
        // space->removeStraight();
    }
    return 0;
}

int Converter::handleOpenTriangleMesh() {
    cerr << "TODO : handleOpenTriangleMesh" << endl;
    int i = 0, count = 0;
    while ( i < this->mesh_list.size() ) {
        if (!this->mesh_list[i]->checkClosed()) {
            count++;
            this->mesh_list.erase(this->mesh_list.begin() + i);
            continue;
        }
        i++;
    }
    printf("\n\n%d meshes have been removed because it is open.\n", count);
    assert(this->mesh_list.size() >= 0);
    return 0;
}

int Converter::export3DS() {
    if (this->convertSpaceToTriangleMesh()) return -1;

    for (int i = 0 ; i < this->mesh_list.size() ; i++){
        TriangleMesh *&triangleMesh = this->mesh_list[i];
        triangleMesh->init();
        cout << "\n\n" << i << "th mesh" << endl;
        if (triangleMesh->checkClosed())
            cout << "this mesh is closed\n\n" << endl;
    }

    MeshExporter::export3DS(this->mesh_list, (paths["versionDir"] + paths["filename"] + ".3DS").c_str());
    return 0;
}
