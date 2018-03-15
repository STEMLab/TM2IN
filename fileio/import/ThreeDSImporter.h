#ifndef THREEDSIMPORTER_H
#define THREEDSIMPORTER_H

#include "Importer.h"

using namespace std;

class ThreeDSImporter : public Importer
{
    public:
        ThreeDSImporter();
        virtual ~ThreeDSImporter();

        TriangleMesh* import(const char*);
    protected:
        long filelength(int f);
        Triangle* makeTriangle(string& input, vector<Vertex*>& vertex);
        string getGroupName(string& input);
        void makeVertex(int id, string& input, Vertex& vt);
    private:
};

#endif // THREEDSIMPORTER_H
