#ifndef MANAGER_H
#define MANAGER_H

#include <logic/check.hpp>
#include <fileio/GenerationWriter.h>
#include <space_maker/SpaceMaker.h>
#include "fileio/Importer.h"
#include "manage/CombineParameter.h"
#include "util.h"

class RoomMaker
{
    public:
        Checker* check;
        Importer* di;
        TriangleMesh* mesh;
        vector<Vertex*> vertex;
        vector<Space*> space_list;
        GenerationWriter* generation_writer;

        RoomMaker(){};
        virtual ~RoomMaker(){};

        int importMesh(const char*);
        int convertTriangleMeshToSpace(double degree);

        virtual int pre_process(double degree) = 0;
        virtual int constructSpace(CombineParameter* ) = 0;
        virtual int finish() = 0;
        virtual int rotateSurfaces() = 0;

        int exportSpaceJSON(string f_path);
        // int exportCombined(string f_path);
        // int importGeneration(string f_path);

        void setImporter(Importer* p_di){ di = p_di; }
        void setChecker(Checker* ch){ check = ch; }
        void setGenerationWriter(GenerationWriter * pw){generation_writer = pw;}

    protected:

    private:
};

#endif // MANAGER_H
