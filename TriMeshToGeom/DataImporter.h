#ifndef DATAIMPORTER_H
#define DATAIMPORTER_H

#include "util.h"
#include "ObjectCollection.h"

class DataImporter
{
    public:
        virtual OBJCollection* import(char*, Checker*) = 0;
    protected:

    private:
};

#endif // DATAIMPORTER_H
