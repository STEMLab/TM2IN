//
// Created by dongmin on 18. 10. 10.
//

#include <Options.h>
#include <restclient-cpp/restclient.h>
#include "InFactoryClient.h"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include "JsonWriter.h"

#define INFACTORY_URL "http://localhost:9797"

#define CREATED 201
#define NOT_FOUND 404
#define FOUND 302


namespace TM2IN {
    namespace detail {
        namespace io {
            static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
            {
                ((std::string*)userp)->append((char*)contents, size * nmemb);
                return size * nmemb;
            }

            void InFactoryClient::make() {
                /* In windows, this will init the winsock stuff */
                curl_global_init(CURL_GLOBAL_ALL);

                string doc_url = INFACTORY_URL + string("/documents/") + docId;
                string if_url = doc_url + "/indoorfeatures/" + ifId;
                string psf_url = doc_url + "/primalspacefeatures/" + psfId;

                string doc_json = string() + "{\n" +
                              "    \"docId\":\"" + docId +"\"" +
                              "}";
                string if_json = string() + "{\n" +
                              "    \"docId\":\"" + docId +"\", " +
                              "    \"id\":\"" + ifId +"\" " +
                              "}";
                string psf_json = string() + "{\n" +
                                  "    \"docId\":\"" + docId +"\", \n" +
                                  "    \"parentId\":\"" + ifId +"\", \n" +
                                  "    \"id\":\"" + psfId +"\" \n" +
                                  "}";

                if (rest_post(doc_url.c_str(), doc_json.c_str()))
                    throw std::runtime_error("\n\nInFactoryClient create Document problem\n\n");

                if (rest_post(if_url.c_str(), if_json.c_str()))
                    throw std::runtime_error("\n\nInFactoryClient create Indoorfeatures problem\n\n");

                if (rest_post(psf_url.c_str(), psf_json.c_str()))
                    throw std::runtime_error("\n\nInFactoryClient create PrimalSpaceFeatures problem\n\n");

                for (Room* room : this->rooms){
                    string cs_url = doc_url + "/cellspace/" + room->geom_id;
                    string cs_json = room_to_cellspace(room);
                    if (rest_post(cs_url.c_str(), cs_json.c_str()))
                        throw std::runtime_error("\n\nInFactoryClient create CellSpace problem\n\n");

                }
                curl_global_cleanup();
            }

            std::string InFactoryClient::getDocumentStr() {
                curl_global_init(CURL_GLOBAL_ALL);
                string doc_url = INFACTORY_URL + string("/documents/") + docId;
                string result = rest_get(doc_url.c_str());
                curl_global_cleanup();
                return result;
            }

            int InFactoryClient::rest_post(const char *url, const char *json) {
                cout << json << endl;
                CURL *curl;
                CURLcode res;
                struct curl_slist *headerlist = nullptr;
                headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

                string result;
                /* get a curl handle */
                curl = curl_easy_init();
                if(curl) {
                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

                    /* Now specify the POST data */
                    curl_easy_setopt(curl, CURLOPT_POST, 1L);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

                    res = curl_easy_perform(curl);
                    /* Check for errors */
                    if(res != CURLE_OK) {
                        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                                curl_easy_strerror(res));
                        return -1;
                    }

                    /* always cleanup */
                    curl_easy_cleanup(curl);
                }
                else{
                    return -1;
                }
                return 0;

            }

            string InFactoryClient::room_to_cellspace(Room *pRoom) {
                string coordinates = "\"SOLID ";
                coordinates += TM2IN::detail::io::room_to_wkt(*pRoom);
                coordinates += "\"";
                string cs_json;
                cs_json += "{\n";
                cs_json += "    \"docId\":\"" + docId +"\", " +
                           "    \"parentId\":\"" + psfId +"\", " +
                           "    \"id\":\"" + pRoom->geom_id +"\",\n";
                cs_json += "        \"geometry\": {\n";
                cs_json += "        \"type\" : \"Solid\",\n";
                cs_json += "        \"coordinates\" : " + coordinates + ",\n";
                cs_json += "        \"properties\" : {\n";
                cs_json += "            \"id\" : \"" + pRoom->geom_id +"g\",\n";
                cs_json += "            \"type\" : \"wkt\"\n";
                cs_json += "        }\n";
                cs_json += "\t\t}\n";
                cs_json += "\t}\n";
                cs_json += "}";
                return cs_json;
            }

            string InFactoryClient::rest_get(const char *url) {
                CURL *curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if(curl) {
                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                    res = curl_easy_perform(curl);
                    if(res != CURLE_OK) {
                        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                                curl_easy_strerror(res));
                        throw std::runtime_error("\n\nInFactoryClient get Document problem\n\n");
                    }
                    curl_easy_cleanup(curl);

                    std::cout << readBuffer << std::endl;
                }
                return readBuffer;
            }
        }
    }
}