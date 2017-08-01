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

        OBJCollection* import(const char*, Checker*);
        static int extractMINtvr(string filename);
    protected:
        void makeTriangle(string& input, vector<Vertex*>& vertex, Triangle& tri);
        string getGroupName(string& input);
        void makeVertex(int id, string& input, Vertex& vt);
        Vertex* findSameVertex(vector<Vertex*>& vertex, Checker* check, Vertex& vt);
    private:
};

#endif // TVRIMPORTER_H
