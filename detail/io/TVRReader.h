//
// Created by dongmin on 18. 7. 16.
//

#ifndef TM2IN_TVRREADER_H
#define TM2IN_TVRREADER_H

#include <fstream>
#include <features/Room.h>

using namespace std;

namespace TM2IN{
    namespace detail{
        namespace io{
            /**
             * @ingroup imp_details
             */
            class TVRReader {
            public:
                TVRReader(std::ifstream& _ifs);
                std::vector<Room*> read();
            private:
                std::ifstream& ifs;

                Wall::Triangle* makeTriangle(string& input, vector<Vertex*>& vertex);
                string getGroupName(string& input);
                Vertex *makeVertex(string &input);
            };

        }
    }
}


#endif //TM2IN_TVRREADER_H
