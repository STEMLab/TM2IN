//
// Created by dongmin on 18. 7. 16.
//

#include "Max3DSReader.h"

#include <sys/stat.h>

#include "features/TriangleMesh.h"
#include "features/Vertex.h"
#include "features/Triangle.h"

namespace TM2IN {
    namespace detail {
        namespace io {
            Max3DSReader::Max3DSReader(FILE* _fp): fp(_fp) {

            }

            std::vector<TriangleMesh *> Max3DSReader::read() {
                int i; //Index variable

                char name[BUFSIZ];

                unsigned short l_chunk_id; //Chunk identifier
                unsigned int l_chunk_length; //Chunk lenght

                unsigned char l_char; //Char variable
                unsigned short l_qty; //Number of elements in each chunk

                unsigned short l_face_flags; //Flag that stores some face information

                vector<TriangleMesh*> meshList;
                TriangleMesh* currentMesh = new TriangleMesh();

                string group_name;
                int f_count =0, v_count = 0;

                while (ftell (this->fp) < this->filelength(fileno (this->fp))) //Loop to scan the whole file
                {
                    //getch(); //Insert this converter for debug (to wait for keypress for each chuck reading)
                    fread (&l_chunk_id, 2, 1, this->fp); //Read the chunk header
                    fread (&l_chunk_length, 4, 1, this->fp); //Read the length of the chunk

                    switch (l_chunk_id)
                    {
                        //----------------- MAIN3DS -----------------
                        // Description: Main chunk, contains all the other chunks
                        // Chunk ID: 4d4d
                        // Chunk Lenght: 0 + sub chunks
                        //-------------------------------------------
                        case MAIN3DS:
                            cout << "0x4D4D : " << l_chunk_length << endl;
                            break;

                            //----------------- EDIT3DS -----------------
                            // Description: 3D Editor chunk, objects layout info
                            // Chunk ID: 3d3d (hex)
                            // Chunk Lenght: 0 + sub chunks
                            //-------------------------------------------
                        case EDIT3DS:
                            cout << "\t0x3D3D : " << l_chunk_length << endl;
                            break;

                            //--------------- EDIT_OBJECT ---------------
                            // Description: Object block, info for each object
                            // Chunk ID: 4000 (hex)
                            // Chunk Lenght: len(object name) + sub chunks
                            //-------------------------------------------
                        case EDIT_OBJECT:
                            cout << "\t\t0x4000 : " << l_chunk_length << endl;
                            if (f_count != 0){
                                meshList.push_back(currentMesh);
                                currentMesh = new TriangleMesh();
                                f_count = 0;
                            }
                            i=0;
                            do
                            {
                                fread (&l_char, 1, 1, this->fp);
                                name[i] = l_char;
                                i++;
                            }while(l_char != '\0');
                            currentMesh->name = name;

                            if (strstr(name, "FC")){
                                cout << "skip furniture" << endl;
                                fseek(this->fp, l_chunk_length -i -6, SEEK_CUR);
                                break;
                            }
                            break;

                            //--------------- OBJ_TRIMESH ---------------
                            // Description: Triangular mesh, contains chunks for 3d mesh info
                            // Chunk ID: 4100 (hex)
                            // Chunk Lenght: 0 + sub chunks
                            //-------------------------------------------

                        case OBJ_TRIMESH:
                            cout << "\t\t\t0x4100 : " << l_chunk_length << endl;
                            break;

                            //--------------- TRI_VERTEXL ---------------
                            // Description: Vertices list
                            // Chunk ID: 4110 (hex)
                            // Chunk Lenght: 1 x unsigned short (number of vertices)
                            //             + 3 x float (vertices coordinates) x (number of vertices)
                            //             + sub chunks
                            //-------------------------------------------
                        case TRI_VERTEXL:
                            cout << "\t\t\t\t0x4110 : " << l_chunk_length << endl;
                            fread (&l_qty, sizeof (unsigned short), 1, this->fp);
                            printf("Number of vertices: %d\n",l_qty);

                            for (i=0; i<l_qty; i++)
                            {
                                Vertex* vt = new Vertex();
                                float temp;
                                fread (&temp, sizeof(float), 1, this->fp);
                                vt->setX(temp);
                                fread (&temp, sizeof(float), 1, this->fp);
                                vt->setY(temp);
                                fread (&temp, sizeof(float), 1, this->fp);
                                vt->setZ(temp);
                                currentMesh->vertices.push_back(vt);
                            }
                            v_count += l_qty;
                            break;

                            //--------------- TRI_FACEL1 ----------------
                            // Description: Polygons (faces) list
                            // Chunk ID: 4120 (hex)
                            // Chunk Lenght: 1 x unsigned short (number of polygons)
                            //             + 3 x unsigned short (polygon points) x (number of polygons)
                            //             + sub chunks
                            //-------------------------------------------

                        case TRI_FACEL1:
                            cout << "\t\t\t\t0x4120 : " << l_chunk_length << endl;
                            fread (&l_qty, sizeof (unsigned short), 1, this->fp);
                            printf("Number of triangles: %d\n",l_qty);
                            f_count = l_qty;
                            for (i=0; i < l_qty; i++)
                            {
                                unsigned short a,b,c;
                                fread (&a, sizeof (unsigned short), 1, this->fp);
                                fread (&b, sizeof (unsigned short), 1, this->fp);
                                fread (&c, sizeof (unsigned short), 1, this->fp);
                                fread (&l_face_flags, sizeof (unsigned short), 1, this->fp);
                                Vertex* va = currentMesh->vertices[a];
                                Vertex* vb = currentMesh->vertices[b];
                                Vertex* vc = currentMesh->vertices[c];
                                Triangle* tri = new Triangle(va, vb, vc);
                                currentMesh->triangles.push_back(tri);
                            }
                            break;
                            /*
                        case KEYF3DS:
                            break;
                             */

                            //----------- Skip unknow chunks ------------
                            //We need to skip all the chunks that currently we don't use
                            //We use the chunk lenght information to set the file pointer
                            //to the same level next chunk
                            //-------------------------------------------
                        default:
                            printf("skip.. 0x%X : %d\n", l_chunk_id, l_chunk_length);
                            fseek(this->fp, l_chunk_length-6, SEEK_CUR);
                    }
                }
                if (f_count != 0){
                    meshList.push_back(currentMesh);
                }
                fclose (this->fp); // Closes the file stream
                return meshList;
            }

            long Max3DSReader::filelength(int f) {
                struct stat buf;

                fstat(f, &buf);

                return(buf.st_size);

            }
        }
    }
}
