#ifndef SPACE_H
#define SPACE_H

#include <vector>
#include <string>

#include "features/Surface.h"
#include "features/Vertex.h"
#include "features/Triangle.h"

using namespace std;

namespace TM2IN {
    class PolyhedralSurface : public Geometry{
    public:
        PolyhedralSurface();

        PolyhedralSurface(string pname);

        virtual ~PolyhedralSurface();

        std::vector<Surface *> surfacesList;
        vector<Vertex *> vertices;

        string name;
        int generation;

        void setName(string _name) {
            name = _name;
        }

        int convertTrianglesToSurfaces(vector<Triangle *> &triangles);

        int surface_strict_validation();

        int surface_easy_validation();

        /**
         * Check whether PolyhedralSurface is closed or not by bfs.
         * @return Boolean
         */
        bool isClosed();

        int updateNormal();

        void sortSurfacesByArea();

        void tagID();

        string asJsonText();

        void setSurfacesList(vector<Surface *> new_list);

        void updateMBB();
    protected:

        void freeSurfaces();

    };
}

#endif // SPACE_H
