//
// Created by dongmin on 18. 10. 10.
//

#ifndef TM2IN_INFACTORYCLIENT_H
#define TM2IN_INFACTORYCLIENT_H

#include <features/Room.h>
#include <Options.h>

using namespace std;

namespace TM2IN {
    namespace detail {
        namespace io {
            static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

            class InFactoryClient {
            public:
                InFactoryClient(std::vector<Room*>& pRooms) : rooms(pRooms){
                    base_url = Options::getInstance()->infactory_url;
                    docId = Options::getInstance()->file_name;
                    ifId = "if1";
                    psfId = "psf1";
                }
                void make();
                std::string getDocumentStr();
            private:
                std::string docId;
                std::string ifId;
                std::string psfId;
                std::string base_url;

                std::vector<Room*>& rooms;

                int rest_post(const char *url, const char *json);
                string rest_get(const char *url);

                string room_to_cellspace(Room *pRoom);
            };
        }
    }
}


#endif //TM2IN_INFACTORYCLIENT_H
