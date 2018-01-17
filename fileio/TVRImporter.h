#ifndef TVRIMPORTER_H
#define TVRIMPORTER_H

#include "fileio/Importer.h"
#include "util.h"

using namespace std;

class TVRImporter : public Importer
{
    public:
        TVRImporter();
        virtual ~TVRImporter();

        TriangleMesh* import(const char*);
        static int extractMINtvr(string filename);
    protected:
        Triangle* makeTriangle(string& input, vector<Vertex*>& vertex);
        string getGroupName(string& input);
        void makeVertex(int id, string& input, Vertex& vt);
        // Vertex* findSameVertex(vector<Vertex*>& vertices, Checker* check, Vertex& vt);
    private:
};

#endif // TVRIMPORTER_H
