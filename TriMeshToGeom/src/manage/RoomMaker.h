#ifndef MANAGER_H
#define MANAGER_H

#include "fileio/Importer.h"
#include "data/ObjectCollection.h"
#include "logic/util.h"

class RoomMaker
{
    public:
        Checker* check;
        Importer* di;
        OBJCollection* objcl;

        virtual int import(const char*) = 0;
        virtual int makeSurfaces(double degree) = 0;
        virtual int makeSimpleSpaces(SpaceMaker* sm) = 0;
        virtual int exportSpaceJSON(string f_path) = 0;
        virtual int exportSimpleSpaceJSON(string f_path) = 0;
        virtual int cleaning(int, double){ return -1;}
        virtual int exportCombined(string f_path) = 0;
        virtual int importCombined(string f_path) = 0;

        void setImporter(Importer* p_di){ di = p_di; }
        void setChecker(Checker* ch){ check = ch; }
        void setProcessWriter(ProcessWriter* pw){objcl->setWriter(pw);}
    protected:

    private:
};

#endif // MANAGER_H
