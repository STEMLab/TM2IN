#ifndef TVRIMPORTER_H
#define TVRIMPORTER_H

#include "fileio/MeshImporter.h"
#include "util.h"

using namespace std;

class TVRImporter : public MeshImporter
{
    public:
        TVRImporter();
        virtual ~TVRImporter();

        std::vector<TriangleMesh*> import(const char*);
        static int extractMINtvr(string fileName);
    protected:

        // Vertex* findSameVertex(vector<Vertex*>& vertices, Checker* check, Vertex& vt);
    private:
};

#endif // TVRIMPORTER_H
