#ifndef TVRIMPORTER_H
#define TVRIMPORTER_H

#include "Importer.h"
#include "util.h"

using namespace std;

class TVRImporter : public Importer
{
    public:
        TVRImporter();
        virtual ~TVRImporter();

        std::vector<TriangleMesh*> import(const char*);
        static int extractMINtvr(string filename);
    protected:
        Triangle* makeTriangle(string& input, vector<Vertex*>& vertex);
        string getGroupName(string& input);
        Vertex * makeVertex(int id, string &input);
        // Vertex* findSameVertex(vector<Vertex*>& vertices, Checker* check, Vertex& vt);
    private:
};

#endif // TVRIMPORTER_H
