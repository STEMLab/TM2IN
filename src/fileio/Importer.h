#ifndef Importer_H
#define Importer_H

#include "logic/util.h"
#include "data/ObjectCollection.h"

class Importer
{
    public:
        virtual OBJCollection* import(const char*, Checker*) = 0;
    protected:

    private:
};

#endif // Importer_H
