#include "features/PolyhedralSurface.h"

#include <cstdio>
#include <queue>
#include <climits>
#include <algorithm>
#include <cmath>
#include <algorithm/mbb.h>
#include <algorithm/compare.h>

#include "detail/io/JsonWriter.h"
#include "compute/unused.h"
#include "features/TriangleMesh.h"
#include "features/HalfEdge.h"

namespace TM2IN {
    PolyhedralSurface::PolyhedralSurface() {
        generation = 0;
        type = TM2IN::IND_TYPE ::PolyhedralSurface;
    }

    PolyhedralSurface::PolyhedralSurface(string pname) :PolyhedralSurface() {
        name = pname;
    }

    PolyhedralSurface::~PolyhedralSurface() {
        //dtor
    }

    int PolyhedralSurface::convertTrianglesToSurfaces(vector<Triangle *> &triangles) {
        vector<Surface *> c_list;
        ull size = triangles.size();
        for (ull i = 0; i < size; i++) {
            TM2IN::Surface * newcp = new Surface(*triangles[i]);
            c_list.push_back(newcp);
        }

        this->surfacesList.insert(this->surfacesList.end(), c_list.begin(), c_list.end());

        cout << "\ndone make Surfaces" << endl;
        return 0;
    }

    int PolyhedralSurface::updateNormal() {
        cout << "\n------------updateNormal------------\n" << endl;
        for (ull i = 0; i < (int) this->surfacesList.size(); i++) {
            Surface * surface = this->surfacesList[i];
            if (!surface->updateNormal()) {
                cout << surface->asJsonText() << endl;
                cout << "Cannot make Normal" << endl;
                exit(-1);
            }

        }
        return 0;
    }

    int PolyhedralSurface::surface_easy_validation() {
        cout << "\n------------- check whether surfaces are valid (easy) --------------\n" << endl;
        for (vector<Surface *>::size_type i = 0; i < this->surfacesList.size();) {
            Surface * pSurface = this->surfacesList[i];
            pSurface->updateMBB();

            if (pSurface->easy_validation()) {
                i++;
            } else {
                cerr << pSurface->asJsonText() << endl;
                return -1;
            }
        }
        return 0;
    }

    int PolyhedralSurface::surface_strict_validation() {
        cout << "\n------------- check whether surfaces are valid --------------\n" << endl;
        for (vector<Surface *>::size_type i = 0; i < this->surfacesList.size();) {
            Surface * pSurface = this->surfacesList[i];
            pSurface->updateMBB();

            if (pSurface->strict_validation()) {
                i++;
            } else {
                cerr << pSurface->asJsonText() << endl;
                return -1;
            }
        }
        return 0;
    }

    void PolyhedralSurface::updateMBB() {
        CGAL::Bbox_3 bbox3 = TM2IN::algorithm::getMBB(this->surfacesList);
        this->mbb->update(bbox3);
    }

    void PolyhedralSurface::freeSurfaces() {
        for (ull i = 0; i < this->surfacesList.size(); i++) {
            delete (this->surfacesList[i]);
        }
        this->surfacesList.clear();
    }


    void PolyhedralSurface::sortSurfacesByArea() {
        sort(this->surfacesList.begin(), this->surfacesList.end(), TM2IN::algorithm::compareArea);
    }

    void PolyhedralSurface::tagID() {
        for (ull i = 0; i < (ull) surfacesList.size(); i++) {
            surfacesList[i]->sf_id = this->name + "_" + to_string(i);
        }
    }

    string PolyhedralSurface::asJsonText() {
        return TM2IN::detail::io::polyhedral_surface_to_json(*this);
    }

    void PolyhedralSurface::setSurfacesList(vector<Surface *> new_list) {
        freeSurfaces();
        this->surfacesList = new_list;
    }

    bool PolyhedralSurface::isClosed() {
        map<Surface *, bool> checked;
        for (Surface *sf : this->surfacesList)
            checked[sf] = false;

        queue<Surface *> wait_queue;
        wait_queue.push(this->surfacesList[0]);
        checked[this->surfacesList[0]] = true;

        int surfaceCount = 0;

        while (wait_queue.size() > 0) {
            Surface * current = wait_queue.front();
            wait_queue.pop();

            surfaceCount += 1;
            for (unsigned int nb = 0; nb < current->getVerticesSize(); nb++) {
                Surface * next_surface = current->exterior_boundary_edge(nb)->getOppositeEdge()->parent;
                if (checked[next_surface]) continue;
                else {
                    checked[next_surface] = true;
                    wait_queue.push(next_surface);
                }
            }
        }

        if (surfaceCount != this->surfacesList.size()) {
            return false;
        } else
            return true;

    }
}
/*
 *
 *
int PolyhedralSurface::removeStraight(){
    cout << "\n------------- removeStraight --------------\n" << endl;
    for (vector<Surface*>::size_type i = 0 ; i < this->surfacesList.size(); ){
        Surface* surface = this->surfacesList[i];
        SurfaceComputation::removeStraight(surface);
        if (surface->strict_validation()){
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

void PolyhedralSurface::rotateSpaceByFloorTo00(){
    cout << " ---------- rotate -------------" << endl;
    sort(this->surfacesList.begin(), this->surfacesList.end(), Surface::compareArea);
    int floor_index = SLC::findFirstSurfaceIndexSimilarWithAxis(this->surfacesList, 2);
    Surface* floor = this->surfacesList[floor_index];
    floor->updateMBB();
    //Plane_3 plane(this->surfacesList[floor_index]->v_list[0]->getCGALPoint(), floor->av_normal);
    //floor->makePlanar(plane);

    Vector_3 vector_z(0,0,1);
    double angle = -TM2IN::cgal::getAngle(floor->normal, vector_z);
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
        Point_3 p = CGAL_User::CGAL_point(this->p_vertexList->at(i));
        p = p.transform(rotateZ);
        this->p_vertexList->at(i)->setCoords(p.x(), p.y(), p.z());
    }
}
*/
