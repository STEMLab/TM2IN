#ifndef Importer_H
#define Importer_H

#include "util.h"
#include "features/TriangleMesh.h"

class Importer
{
    public:
        virtual vector<TriangleMesh*> import(const char *filePath) = 0;
    protected:

    private:
};

#endif // Importer_H
