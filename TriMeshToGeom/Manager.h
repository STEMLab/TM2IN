#ifndef MANAGER_H
#define MANAGER_H

#include "Importer.h"
#include "ObjectCollection.h"
#include "util.h"

class Manager
{
    public:
        Checker* check;
        Importer* di;
        OBJCollection* objcl;

        virtual int import(const char*) = 0;
        virtual int makeSurfaces() = 0;
        virtual void makeSolids() = 0;
        virtual int exportJSON(string f_path) = 0;
        virtual int cleaning(){ return -1;}
        virtual int exportCombined(string f_path) = 0;
        virtual int importCombined(string f_path) = 0;

        void setImporter(Importer* p_di){ di = p_di; }
        void setChecker(Checker* ch){ check = ch; }
    protected:

    private:
};

#endif // MANAGER_H
