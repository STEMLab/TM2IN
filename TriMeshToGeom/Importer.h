#ifndef Importer_H
#define Importer_H

#include "util.h"
#include "ObjectCollection.h"

class Importer
{
    public:
        virtual OBJCollection* import(const char*, Checker*) = 0;
    protected:

    private:
};

#endif // Importer_H
