#ifndef THREEDSIMPORTER_H
#define THREEDSIMPORTER_H

#include "fileio/MeshImporter.h"


using namespace std;

class ThreeDSImporter : public MeshImporter
{
    public:
        ThreeDSImporter();
        virtual ~ThreeDSImporter();

        vector<TriangleMesh*> import(const char*);
    protected:
        long filelength(int f);
        Triangle* makeTriangle(string& input, vector<Vertex*>& vertex);
        string getGroupName(string& input);
        void makeVertex(int id, string& input, Vertex& vt);
    private:
};

#endif // THREEDSIMPORTER_H
