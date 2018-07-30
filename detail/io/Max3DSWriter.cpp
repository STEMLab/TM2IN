//
// Created by dongmin on 18. 7. 19.
//

#include "Max3DSWriter.h"
#include "Max3DSReader.h"

#include "features/Room.h"
#include "features/RoomBoundary/TriangleMesh.h"
#include "features/Vertex.h"
#include "features/Triangle.h"

using namespace std;

namespace TM2IN{
    namespace detail{
        namespace io{
            int Max3DSWriter::write(vector<Room*> rooms) {

                unsigned short chunk_id = 0x4D4D;
                unsigned int chunk_length = 0;
                fwrite(&chunk_id, 2, 1, pFile);
                fwrite(&chunk_length, 4, 1, pFile);

                chunk_id = 0x3D3D;
                chunk_length = 0;
                fwrite(&chunk_id, 2, 1, pFile);
                fwrite(&chunk_length, 4, 1, pFile);
                unsigned int wholeLength = 0;

                for (int groupI = 0 ; groupI < rooms.size(); groupI++){
                    Room* room = rooms[groupI];
                    RoomBoundary::TriangleMesh* tm = room->getTm_boundary();
                    vector<Triangle*> triangles = tm->getTriangleList();
                    vector<Vertex*> vertices = tm->extractVerticesList();
                    vector<vector<int>> triangleIndicies;
                    for (int triI = 0 ; triI < triangles.size(); triI++){
                        Triangle* triangle = triangles[triI];
                        vector<int> triangleIndex;
                        for (int vi = 0 ; vi < 3 ; vi++){
                            Vertex* vt = triangle->vertex(vi);
                            int index = 0;
                            for (; index < vertices.size(); index++){
                                if (vertices[index] == vt)
                                    break;
                            }
                            assert (vertices.size() != index);
                            triangleIndex.push_back(index);
                        }
                        triangleIndicies.push_back(triangleIndex);
                    }

                    unsigned int VERTICES_LIST_LENGTH = sizeof(unsigned short) + sizeof(unsigned int) + // Vertex : chunk_id and chunk_length
                                                        sizeof(unsigned short) + // Vertices number
                                                        sizeof(float) * 3 * vertices.size(); // Vertices List
                    unsigned int FACES_DESCRIPTION_LENGTH = sizeof(unsigned short) + sizeof(unsigned int) + // Polygon : chunk_id and chunk_length
                                                            sizeof(unsigned short) + // Polygons number
                                                            sizeof(unsigned short) * 4 * triangleIndicies.size(); // Polygons List
                    unsigned int TRIANGULAR_MESH_LENGTH = sizeof(unsigned short) + sizeof(unsigned int) + //OBJ_TRIMESH
                                                          VERTICES_LIST_LENGTH + FACES_DESCRIPTION_LENGTH;

                    chunk_id = EDIT_OBJECT;
                    chunk_length = sizeof(unsigned short) + sizeof(unsigned int) +
                                   (unsigned short)room->name.size() + 1 +
                                   // Triangle Mesh : chunk_id and chunk_length
                                   TRIANGULAR_MESH_LENGTH;
                    wholeLength += chunk_length;

                    fwrite(&chunk_id, 2, 1, pFile);
                    fwrite(&chunk_length, 4, 1, pFile);

                    const char* name = room->name.c_str();
                    for (int i = 0 ; name[i] != 0 ; i++){
                        fwrite(&name[i], sizeof(char), 1, pFile);
                    }
                    char temp = 0;
                    fwrite(&temp, sizeof(char),1, pFile); //NULL

                    // Triangle Mesh
                    chunk_id = OBJ_TRIMESH;
                    chunk_length = TRIANGULAR_MESH_LENGTH;
                    fwrite(&chunk_id, 2, 1, pFile);
                    fwrite(&chunk_length, 4, 1, pFile);

                    // Vertices List
                    chunk_id = TRI_VERTEXL;
                    chunk_length = VERTICES_LIST_LENGTH;
                    fwrite(&chunk_id, 2, 1, pFile);
                    fwrite(&chunk_length, 4, 1, pFile);
                    unsigned short sizeOfVertices = (unsigned short)vertices.size();
                    fwrite(&sizeOfVertices, sizeof(unsigned short), 1, pFile);
                    for (int vi = 0 ; vi < vertices.size(); vi++){
                        float x = (float)vertices[vi]->x();
                        float y = (float)vertices[vi]->y();
                        float z = (float)vertices[vi]->z();
                        fwrite(&x, sizeof(float), 1, pFile);
                        fwrite(&y, sizeof(float), 1, pFile);
                        fwrite(&z, sizeof(float), 1, pFile);
                    }

                    // Triangle List
                    chunk_id = TRI_FACEL1;
                    chunk_length = FACES_DESCRIPTION_LENGTH;
                    fwrite(&chunk_id, 2, 1, pFile);
                    fwrite(&chunk_length, 4, 1, pFile);
                    unsigned short sizeOfTriangles = (unsigned short)triangleIndicies.size();
                    fwrite(&sizeOfTriangles, sizeof(unsigned short), 1, pFile);
                    for (int tr = 0 ; tr < triangleIndicies.size(); tr++){
                        for (int i = 0 ; i < 3 ; i++){
                            fwrite(&triangleIndicies[tr][i], sizeof(unsigned short), 1, pFile);
                        }
                        unsigned short temp = 1;
                        fwrite(&temp, sizeof(unsigned short), 1, pFile);
                    }
                }

                fseek(pFile, sizeof(unsigned short) * 2 + sizeof(unsigned int), SEEK_SET);
                wholeLength += sizeof(unsigned short) + sizeof(unsigned int);
                fwrite(&wholeLength, sizeof(unsigned short), 1, pFile);

                fseek(pFile, sizeof(unsigned short) , SEEK_SET);
                wholeLength += sizeof(unsigned short) + sizeof(unsigned int);
                fwrite(&wholeLength, sizeof(unsigned short), 1, pFile);

                fclose(pFile);

                return 0;
            }

            Max3DSWriter::Max3DSWriter(FILE *pFile) : pFile(pFile) { }

        }
    }
}


