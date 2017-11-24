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

        int import(const char*);
        virtual int pre_process(double degree) = 0;
        virtual int makeSimpleSpaces(SpaceMaker* sm) = 0;
        virtual int constructSpace(int, double){ return -1;}
        int exportSpaceJSON(string f_path);
        int exportSimpleSpaceJSON(string f_path);
        int exportCombined(string f_path);
        int importCombined(string f_path);

        void setImporter(Importer* p_di){ di = p_di; }
        void setChecker(Checker* ch){ check = ch; }
        void setProcessWriter(ProcessWriter* pw){objcl->setWriter(pw);}

    protected:

    private:
};

#endif // MANAGER_H
