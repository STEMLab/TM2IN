#ifndef TVRIMPORTER_H
#define TVRIMPORTER_H

#include "DataImporter.h"
#include "util.h"

using namespace std;

class TVRImporter : public DataImporter
{
    public:
        TVRImporter();
        virtual ~TVRImporter();

        OBJCollection* import(char*, Checker*);
        static int extractMINtvr(string filename);
    protected:
        void makeTriangle(string& input, vector<Vertex*>& vertex, Triangle& tri);
        string getGroupName(string& input);
        void makeVertex(string& input, Vertex& vt);
        Vertex* findSameVertex(vector<Vertex*>& vertex, Checker* check, Vertex& vt);
    private:
};

#endif // TVRIMPORTER_H
