#ifndef MANAGER_H
#define MANAGER_H

#include <logic/check.hpp>
#include <fileio/GenerationWriter.h>
#include "fileio/Importer.h"
#include "util.h"

class RoomMaker
{
    public:
        Importer* di;
        TriangleMesh* mesh;
        vector<Vertex*> vertices;
        vector<Space*> spaceList;
        GenerationWriter* generation_writer;

        RoomMaker(){};
        virtual ~RoomMaker(){};

        int importMesh(const char*);
        int convertTriangleMeshToSpace(double degree);

        virtual int pre_process() = 0;
        virtual int constructSpace() = 0;
        virtual int finish() = 0;
        virtual int rotateSurfaces() = 0;

        int exportSpaceJSON(string f_path);
        // int exportCombined(string f_path);
        // int importGeneration(string f_path);

        void setImporter(Importer* p_di){ di = p_di; }
        void setGenerationWriter(GenerationWriter * pw){generation_writer = pw;}

    protected:

    private:
};

#endif // MANAGER_H
