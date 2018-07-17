#include "features/PolyhedralSurface.h"

#include "compute/SurfacesListComputation.h"
#include "compute/VertexComputation.h"

#include <cstdio>
#include <queue>
#include <climits>
#include <algorithm>
#include <cmath>
#include <compute/SurfaceComputation.h>
#include "cgal/SurfaceIntersection.h"
#include "HalfEdge.h"

PolyhedralSurface::PolyhedralSurface(){
    generation = 0;
}

PolyhedralSurface::PolyhedralSurface(string pname)
{
    name = pname;
    generation = 0;
}

PolyhedralSurface::~PolyhedralSurface()
{
    //dtor
}


int PolyhedralSurface::convertTrianglesToSurfaces(vector<Triangle*>& triangles){
    vector<Surface*> c_list;
    ull size = triangles.size();
    for (ull i = 0 ; i < size; i++){
        Surface* newcp = new Surface(*triangles[i]);
        c_list.push_back(newcp);
    }

    this->surfacesList.insert(this->surfacesList.end(), c_list.begin(), c_list.end());

    cout << "\ndone make Surfaces" << endl;
    return 0;
}


int PolyhedralSurface::mergeSurface() {
    cout << "Combine Surfaces" << endl;
    vector<Surface*> new_poly_list;
    bool hasMerged = TMIC::mergeSurfaces(this->surfacesList, new_poly_list);
    freeSurfaces();
    this->surfacesList = new_poly_list;
    return hasMerged;
}

int PolyhedralSurface::updateNormal(){
    cout << "\n------------updateNormal------------\n" << endl;
    for (ull i = 0 ; i < (int)this->surfacesList.size() ; i++)
    {
        Surface* surface = this->surfacesList[i];
        if (!surface->updateNormal())
        {
            cout << surface->toJSONString() <<endl;
            cout << "Cannot make Normal" << endl;
            exit(-1);
        }

    }
    return 0;
}


int PolyhedralSurface::simplifySegment(){
    bool hasSimplified = false;
    cout << "\n------------simplifySegment------------\n" << endl;
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareLength);
    ull sizeOfSurfaces = this->surfacesList.size();

    for (ull i = 0 ; i < sizeOfSurfaces; i++)
        assert((int) this->surfacesList[i]->getVerticesSize() >= 3);

    for (ull i = 0 ; i < sizeOfSurfaces - 1; i++){
        Surface *&surfaceI = this->surfacesList[i];
        if (!surfaceI->isValid()) continue;
        printProcess(i, sizeOfSurfaces, "");
        for (ull j = i + 1; j < sizeOfSurfaces ; j++){
            Surface *&surfaceJ = this->surfacesList[j];
            if (!surfaceI->isValid()) break;
            if (!surfaceJ->isValid()) continue;
            if (!CGALCalculation::isIntersect_BBOX(surfaceI, surfaceJ)) continue;
            while (TMIC::simplifyLineSegment(this->surfacesList[i], this->surfacesList[j]) == 0){
                cout << "Simplification" << endl;
                hasSimplified = true;
                if (!surfaceI->isValid() || !surfaceJ->isValid()) break;
            }
        }
    }
    sizeOfSurfaces = this->surfacesList.size();

    bool hasRemoved = false;
    for (int i = sizeOfSurfaces - 1; i >= 0 ; i--){
        if (this->surfacesList[i]->isValid()){
        }
        else{
            hasRemoved = true;
            cout << "remove" << endl;
            delete this->surfacesList[i];
            this->surfacesList.erase(this->surfacesList.begin() + i);
        }
    }

    if (hasRemoved)
        hasSimplified = (simplifySegment() || hasSimplified);

    return hasSimplified;
}

int PolyhedralSurface::checkSurfaceValid() {
    cout << "\n------------- check whether surface is valid --------------\n" << endl;
    for (vector<Surface*>::size_type i = 0 ; i < this->surfacesList.size(); )
    {
        Surface* surface = this->surfacesList[i];
        surface->updateMBB();

        if (surface->isValid()){
            i++;
        }
        else{
            return -1;
        }
    }
    return 0;
}


int PolyhedralSurface::removeStraight(){
    cout << "\n------------- removeStraight --------------\n" << endl;
    for (vector<Surface*>::size_type i = 0 ; i < this->surfacesList.size(); ){
        Surface* surface = this->surfacesList[i];
        SurfaceComputation::removeStraight(surface);
        if (surface->isValid()){
            i++;
        }
        else{
            delete surface;
            this->surfacesList.erase(this->surfacesList.begin() + i);
            Checker::num_of_invalid += 1;
            cout << "Erase invalid surface" << endl;
        }
    }
    return 0;
}

int PolyhedralSurface::translateSpaceToOrigin(){
    cout << "\n------------- translateSpaceToOrigin --------------\n" << endl;

    updateMBB();
    double diff[3];
    for (int i = 0 ; i < 3 ; i++){
        diff[i] = -this->mbb.min(i);
    }

    for (ull i = 0 ; i < this->surfacesList.size() ; i++)
    {
        this->surfacesList[i]->translate(diff);
    }

    for (ull i = 0 ; i < this->vertices.size() ; i++){
        this->vertices[i]->translate(diff);
    }

    return 0;
}

void PolyhedralSurface::updateMBB(){
    mbb = TMIC::getMBB(this->surfacesList);
}

void PolyhedralSurface::freeSurfaces(){
    for (ull i = 0 ; i < this->surfacesList.size() ; i++)
    {
        delete(this->surfacesList[i]);
    }
    this->surfacesList.clear();
}


int PolyhedralSurface::checkDuplicateVertexInSurfaces() {
    for (unsigned int s_i = 0 ; s_i < this->surfacesList.size() ;s_i++){
        if (surfacesList[s_i]->checkDuplicate()){
            cout << "it has duplicate Vertex" << endl;
            return -1;
        }
    }
    return 0;
}

void PolyhedralSurface::sortSurfacesByArea() {
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
}

void PolyhedralSurface::tagID() {
    SurfacesListComputation::tagID(this->surfacesList);
}

void PolyhedralSurface::triangulateSurfaces() {
    for (unsigned int sfID = 0 ; sfID < this->surfacesList.size(); sfID++) {
        Surface* pSurface = this->surfacesList[sfID];
        vector<Triangle*> triangles;
        if (SurfaceComputation::triangulate(pSurface, triangles)){
            cerr << "Triangulation Error" << endl;
            exit(-1);
        }
        assert(triangles.size());
        pSurface->triangulation = triangles;
    }
}

int PolyhedralSurface::checkSelfIntersection() {
    int count = 0;
    for (unsigned int sfID = 0 ; sfID < this->surfacesList.size(); sfID++) {
        Surface* pSurface = this->surfacesList[sfID];
        if (SurfaceIntersection::checkSelfIntersection(pSurface)){
            cerr << "Self Intersection in Surface " << sfID << endl;
            cerr << pSurface->toJSONString() << endl;
            count++;
        }
        else {
        }
    }
    cout << this->name << " Self Intersection Count : " << count << endl;
    return 0;
}

vector<Triangle *> PolyhedralSurface::getTriangulation() {
    vector<Triangle *> triangles;
    for (unsigned int sfID = 0 ; sfID < this->surfacesList.size(); sfID++) {
        Surface* pSurface = this->surfacesList[sfID];
        assert(pSurface->triangulation.size());
        int index = 0;
        for (Triangle* triangle : pSurface->triangulation){
            triangle->sf_id = to_string(sfID) + "_" + to_string(index++);
        }
        triangles.insert(triangles.end(),pSurface->triangulation.begin(),pSurface->triangulation.end());
    }
    return triangles;
}

double PolyhedralSurface::getAverageError() {
    double errorSum = 0.0;
    for (Surface* surface : this->surfacesList){
        errorSum += TMIC::computeError(surface);
        if (errorSum > 0)
            cout << errorSum << endl;
    }
    return errorSum;
}

/*

void PolyhedralSurface::rotateSpaceByFloorTo00(){
    cout << " ---------- rotate -------------" << endl;
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    int floor_index = SLC::findFirstSurfaceIndexSimilarWithAxis(this->surfacesList, 2);
    Surface* floor = this->surfacesList[floor_index];
    floor->updateMBB();
    //Plane_3 plane(this->surfacesList[floor_index]->v_list[0]->getCGALPoint(), floor->av_normal);
    //floor->makePlanar(plane);

    Vector_3 vector_z(0,0,1);
    double angle = -CGALCalculation::getAngle(floor->normal, vector_z);
    if (angle == 0.0){
        cout << "angle is 0.0" << endl;
        return;
    }

    Vector_3 unit_vector = CGAL::cross_product(vector_z, floor->normal);
    unit_vector = unit_vector / sqrt(unit_vector.squared_length());
    cout << "rotate " << angle << ", " << unit_vector.squared_length()<< endl;
    assert(unit_vector.squared_length() < 1.000001 && unit_vector.squared_length() > 0.99999);

    double cos_value = cos(angle * PI /180.0);
    double sin_value = sin(angle * PI /180.0);

    double ux = unit_vector.x();
    double uy = unit_vector.y();
    double uz = unit_vector.z();

    Transformation rotateZ(cos_value + ux*ux *(1-cos_value), ux*uy*(1-cos_value) - (uz * sin_value), ux*uz*(1-cos_value) + uy*sin_value,
                            uy*ux*(1-cos_value) + uz * sin_value,cos_value + uy*uy*(1-cos_value), uy*uz*(1-cos_value)- (ux*sin_value),
                            uz*ux*(1-cos_value)-uy*sin_value , uz*uy*(1-cos_value) + ux * sin_value, cos_value + uz*uz*(1-cos_value),
                            1);

    for (ull i = 0 ; i < this->p_vertexList->size() ; i++){
        Point_3 p = CGAL_User::getCGALPoint(this->p_vertexList->at(i));
        p = p.transform(rotateZ);
        this->p_vertexList->at(i)->setCoords(p.x(), p.y(), p.z());
    }
}
*/
