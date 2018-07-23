#include <io/tvr.h>
#include <io/max3ds.h>
#include <io/collada.h>
#include <io/json.h>
#include "Converter.h"

#include "compute/SurfacesListComputation.h"
#include "features/HalfEdge.h"

Converter::Converter(Options& op) : options(op) {}

int Converter::start() {
    // import mesh data
    importMesh();

    // generation writer
    if (options.generator) generation_writer = new GenerationWriter(options.output_dir);
    else generation_writer = NULL;

    // construct graph
    initTriangleMesh();
    if (partitionTriangleMeshByComponent()){
        throw std::runtime_error("Converter : partitionTriangleMeshByComponent error");
    }

    // mesh validation
    // if (handleOpenTriangleMesh()) return -1;

    // remove Furniture
    if (remainSelectedMesh(options.selected)) return -1;

    // Triangle Mesh to PolyhedralSurface
    if (convertTriangleMeshToSpace()) return -1;

    return 0;
}

int Converter::run() {
    mergeSurfaces();
    validate();

    if (options.polygonizer_mode > 0) // 1 or 2 or 3
        polygonize();

    return 0;
}

int Converter::finish() {
    this->tagID();
    this->exportSpace();

    return 0;
}

int Converter::exportSpace() {
    //JSON
    TM2IN::io::exportJSON(options.output_dir + "surfaces.json", this->spaceList);

    if (options.output_3ds || options.output_tvr){
        convertSpaceToTriangleMesh();
        initTriangleMesh();
        for (int i = 0 ; i < this->mesh_list.size() ; i++){
            TriangleMesh *&triangleMesh = this->mesh_list[i];
            if (triangleMesh->checkClosed())
                cout << "this mesh is closed\n\n" << endl;
            else
                cerr << "this mesh is not closed\n\n" << endl;
        }
    }

    //TVR
    if (options.output_tvr){

    }

    //3DS
    if (options.output_3ds){
        TM2IN::io::export3DS((options.output_dir + options.file_name + ".3DS").c_str(), this->mesh_list);
    }
    return 0;
}

int Converter::convertSpaceToTriangleMesh(){
    this->mesh_list.clear();
    for (int spaceID = 0 ; spaceID < this->spaceList.size() ; spaceID++){
        PolyhedralSurface* space = this->spaceList[spaceID];
        vector<Triangle*> triangleList;
        for (unsigned int sfID = 0 ; sfID < space->surfacesList.size(); sfID++) {
            Surface* pSurface = space->surfacesList[sfID];
            vector<Triangle*> triangulation = pSurface->getTriangulation();
            triangleList.insert(triangleList.end(), triangulation.begin(), triangulation.end());
        }

        for (Triangle* triangle : triangleList){
            vector<HalfEdge*> edges = triangle->getExteriorBoundary();
            for (HalfEdge* he : edges){
                he->oppositeEdge = NULL;
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


void Converter::tagID() {
    for (ull it = 0 ; it < this->spaceList.size() ; it++) {
        PolyhedralSurface *space = this->spaceList[it];
        space->tagID();
    }
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

void Converter::printInputDataSpec() {
    vector<Surface*> surfaces;

    for (ull it = 0 ; it <this->mesh_list.size() ; it++){
        for (Triangle* triangle : this->mesh_list[it]->triangles)
            surfaces.push_back(triangle);
    }

    int trianglesCount = surfaces.size();
    double area = TMIC::getAverageSize(surfaces);
    CGAL::Bbox_3 mbb;
    mbb = TMIC::getMBB(surfaces);

    cout << "\n\nTriangles : " << trianglesCount << endl;
    cout << "Bbox : " << mbb << endl;
    cout << "Area : " << area << endl;
    cout << "\n\n" << endl;
}
