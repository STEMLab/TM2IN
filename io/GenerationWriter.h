#ifndef GenerationWriter_H_INCLUDED
#define GenerationWriter_H_INCLUDED

#include "features/Room.h"
#include "util.h"
#include <fstream>

using namespace std;
using namespace TM2IN;

namespace TM2IN {
    namespace io {
        class GenerationWriter{
        private:
            GenerationWriter() {}

            static GenerationWriter *instance;

            int mode = 0; // 0 : wait, 1 : running
            string directory_path;
            string room_name;
        public:
            static GenerationWriter *getInstance() {
                if (!instance) {
                    instance = new GenerationWriter();
                }
                return instance;
            }

            void start(Room* room);

            void write(RoomBoundary::TriangulatedSurfaceMesh* tsm);
            void write(vector<Wall::TriangulatedSurface*>& ts_list);

            void close();
        };
    }
}


#endif // GenerationWriter_H_INCLUDED
