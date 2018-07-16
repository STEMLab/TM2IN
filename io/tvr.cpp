//
// Created by dongmin on 18. 7. 16.
//

#include <detail/io/TVRReader.h>
#include "tvr.h"

namespace TM2IN {
    namespace io {
        vector<TriangleMesh *> importTVR(const char *filePath) {
            ifstream fin;
            fin.open(filePath);

            assert(fin);

            TM2IN::detail::io::TVRReader reader(fin);
            return reader.read();
        }
    }
}


/*
Vertex* TVRImporter::findSameVertex(vector<Vertex*>& vertices, Checker* check, Vertex& vt){
    vector<Vertex*>::iterator it, low;
    low = std::lower_bound(vertices.begin(), vertices.end(), vt.x() - check->thresholdVertex * 2, CompareVertex_X() );

    for (it = low ; it != vertices.end() ; it++){
        double diff = check->compare_vertex( (*it), &vt);
        if (diff > 0){
            break;
        }
        else if (diff < 0){
            continue;
        }
        else{
            return *it;
        }
    }

    Vertex* new_vt = new Vertex(vt);
    vertices.insert(it, new_vt);

    return new_vt;
}
*/
