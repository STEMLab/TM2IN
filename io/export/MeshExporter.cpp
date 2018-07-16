//
// Created by dongmin on 18. 3. 30.
//

#include "MeshExporter.h"
#include "fileio/import/ThreeDSImporter.h"

int MeshExporter::export3DS(vector<TriangleMesh *> meshList, const char *filePath) {
    FILE* pFile;
    pFile= fopen(filePath, "w");
    if (!pFile) {
        cerr << "cannot open export3DS";
        return -1;
    }

    unsigned short chunk_id = 0x4D4D;
    unsigned int chunk_length = 0;
    fwrite(&chunk_id, 2, 1, pFile);
    fwrite(&chunk_length, 4, 1, pFile);

    chunk_id = 0x3D3D;
    chunk_length = 0;
    fwrite(&chunk_id, 2, 1, pFile);
    fwrite(&chunk_length, 4, 1, pFile);
    unsigned int wholeLength = 0;

    for (int groupI = 0 ; groupI < meshList.size(); groupI++){
        vector<vector<int>> triangleIndicies;
        for (int triI = 0 ; triI < meshList[groupI]->triangles.size(); triI++){
            Triangle* triangle = meshList[groupI]->triangles[triI];
            vector<int> triangleIndex;
            for (int vi = 0 ; vi < 3 ; vi++){
                Vertex* vt = triangle->vertex(vi);
                int index = 0;
                for (; index < meshList[groupI]->vertices.size(); index++){
                    if (meshList[groupI]->vertices[index] == vt)
                        break;
                }
                assert (meshList[groupI]->vertices.size() != index);
                triangleIndex.push_back(index);
            }
            triangleIndicies.push_back(triangleIndex);
        }

        unsigned int VERTICES_LIST_LENGTH = sizeof(unsigned short) + sizeof(unsigned int) + // Vertex : chunk_id and chunk_length
                                            sizeof(unsigned short) + // Vertices number
                                            sizeof(float) * 3 * meshList[groupI]->vertices.size(); // Vertices List
        unsigned int FACES_DESCRIPTION_LENGTH = sizeof(unsigned short) + sizeof(unsigned int) + // Polygon : chunk_id and chunk_length
                                                sizeof(unsigned short) + // Polygons number
                                                sizeof(unsigned short) * 4 * triangleIndicies.size(); // Polygons List
        unsigned int TRIANGULAR_MESH_LENGTH = sizeof(unsigned short) + sizeof(unsigned int) + //OBJ_TRIMESH
                                              VERTICES_LIST_LENGTH + FACES_DESCRIPTION_LENGTH;

        chunk_id = EDIT_OBJECT;
        chunk_length = sizeof(unsigned short) + sizeof(unsigned int) +
                       (unsigned short)meshList[groupI]->name.size() + 1 +
                       // Triangle Mesh : chunk_id and chunk_length
                       TRIANGULAR_MESH_LENGTH;
        wholeLength += chunk_length;

        fwrite(&chunk_id, 2, 1, pFile);
        fwrite(&chunk_length, 4, 1, pFile);

        const char* name = meshList[groupI]->name.c_str();
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
        unsigned short sizeOfVertices = (unsigned short)meshList[groupI]->vertices.size();
        fwrite(&sizeOfVertices, sizeof(unsigned short), 1, pFile);
        for (int vi = 0 ; vi < meshList[groupI]->vertices.size(); vi++){
            float x = (float)meshList[groupI]->vertices[vi]->x();
            float y = (float)meshList[groupI]->vertices[vi]->y();
            float z = (float)meshList[groupI]->vertices[vi]->z();
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
