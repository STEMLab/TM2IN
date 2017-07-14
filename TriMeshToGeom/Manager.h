#ifndef MANAGER_H
#define MANAGER_H

#include "DataImporter.h"
#include "ObjectCollection.h"
#include "util.h"

class Manager
{
    public:
        Checker* check;
        DataImporter* di;
        OBJCollection* objcl;

        virtual int import(char*) = 0;
        virtual void makeSurfaces() = 0;
        virtual void makeSolids() = 0;
        virtual int exportJSON(string f_path) = 0;
        virtual void cleaning(){ }
        void setImporter(DataImporter* p_di){ di = p_di; }
        void setChecker(Checker* ch){ check = ch; }
    protected:

    private:
};

#endif // MANAGER_H
