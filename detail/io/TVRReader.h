//
// Created by dongmin on 18. 7. 16.
//

#ifndef TM2IN_TVRREADER_H
#define TM2IN_TVRREADER_H

#include <fstream>
#include <features/TriangleMesh.h>

namespace TM2IN{
    namespace detail{
        namespace io{
            class TVRReader {
            public:
                TVRReader(std::ifstream& _ifs);
                std::vector<TriangleMesh *> read();
            private:
                std::ifstream& ifs;

                Triangle* makeTriangle(string& input, vector<Vertex*>& vertex);
                string getGroupName(string& input);
                Vertex * makeVertex(int id, string &input);
            };

        }
    }
}


#endif //TM2IN_TVRREADER_H
