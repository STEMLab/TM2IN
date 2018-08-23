//
// Created by dongmin on 18. 7. 16.
//

#include "detail/io/ColladaReader.h"
#include "detail/io/Max3DSReader.h"
#include "detail/io/Max3DSWriter.h"
#include "max3ds.h"

#include <iostream>

using namespace std;

namespace TM2IN{
    namespace io{
        std::vector<Room *> import3DS(const char *filePath) {
            FILE *l_file;
            if ((l_file=fopen (filePath, "rb") ) == NULL){
                throw std::runtime_error(string(filePath) + " cannot be open in import3DS");
            }

            TM2IN::detail::io::Max3DSReader reader(l_file);
            return reader.read();
        }


        int export3DS(const char *filePath, vector<Room*> rooms) {
            FILE* pFile;
            pFile= fopen(filePath, "w");
            if (!pFile) {
                throw std::runtime_error(string(filePath) + " cannot be open in export3DS");
            }

            TM2IN::detail::io::Max3DSWriter writer(pFile);
            return writer.write(rooms);
        }

    }
}
