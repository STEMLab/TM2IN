//
// Created by dongmin on 18. 7. 16.
//

#ifndef TM2IN_TVR_H
#define TM2IN_TVR_H

#include <features/TriangleMesh.h>

namespace TM2IN{
    namespace io{
        vector<TriangleMesh*> importTVR(const char *filePath);
        void convertTVRtoMinimumTVR(const char *input_file_path, (const char *output_file_path)
    }
}


#endif //TM2IN_TVR_H
