/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 * Tutorial 4: 3d engine - 3ds models loader
 *
 * Include File: 3dsloader.cpp
 *
 */

/*
Linux port by Panteleakis Ioannis
mail: pioann@csd.auth.gr

just run: make and you are done.
of course you may need to change the makefile
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "Object_Type.hpp"
#include "3dsloader.h"

#include <sys/stat.h>

using namespace std;


long filelength(int f)
{
    struct stat buf;

    fstat(f, &buf);

    return(buf.st_size);
}

vector<pair<string, TriangleSpace*>> Load3DS (char *p_filename)
{
    vector<pair<string, TriangleSpace*>> obj_list;
    TriangleSpace *p_object = NULL;

	int i; //Index variable

    int temp_vertex[3] = {0,0,0};

	FILE *l_file; //File pointer
    char name[20];

	unsigned short l_chunk_id; //Chunk identifier
	unsigned int l_chunk_lenght; //Chunk lenght

	unsigned char l_char; //Char variable
	unsigned short l_qty; //Number of elements in each chunk

	unsigned short l_face_flags; //Flag that stores some face information

    if ( (l_file=fopen (p_filename, "rb") ) == NULL) return obj_list;

	while (ftell (l_file) < filelength (fileno (l_file))) //Loop to scan the whole file
	//while(!EOF)
	{
		//getch(); //Insert this command for debug (to wait for keypress for each chuck reading)

		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
		//printf("ChunkID: %x\n",l_chunk_id);
		fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
		//printf("ChunkLenght: %x\n",l_chunk_lenght);

		switch (l_chunk_id)
        {
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d:
			break;

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x3d3d:
			break;

			//--------------- EDIT_OBJECT ---------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
			case 0x4000:
				i=0;
				p_object = new TriangleSpace();
				do
				{
					fread (&l_char, 1, 1, l_file);
                    name[i] = l_char;
                    i++;
                }while(l_char != '\0' && i<20);
                cout<< name << endl;

                if (strstr(name, "FC")){
                    cout << "skip furniture" << endl;
                    fseek(l_file, l_chunk_lenght -i -6, SEEK_CUR);
                    break;
                }
			break;

			//--------------- OBJ_TRIMESH ---------------
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4100:
                printf("TRIMESH\n");
                break;

			//--------------- TRI_VERTEXL ---------------
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Lenght: 1 x unsigned short (number of vertices)
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
			case 0x4110:
                if (p_object == NULL) {
                    cout << "4110 error" << endl;
                    exit(1);
                }

				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                p_object->vertices_qty = l_qty;

                printf("Number of vertices: %d\n",l_qty);
                p_object->vertex = (Vertex*)malloc(sizeof(Vertex) * l_qty);

                for (i=0; i<l_qty; i++)
                {
					fread (&p_object->vertex[i].x, sizeof(float), 1, l_file);
                    fread (&p_object->vertex[i].y, sizeof(float), 1, l_file);
					fread (&p_object->vertex[i].z, sizeof(float), 1, l_file);
				}
				break;

			//--------------- TRI_FACEL1 ----------------
			// Description: Polygons (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk Lenght: 1 x unsigned short (number of polygons)
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub chunks
			//-------------------------------------------

			case 0x4120:
                if (p_object == NULL) {
                    cout << "4120 error" << endl;
                    exit(1);
                }

				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                p_object->polygons_qty = l_qty;
                printf("Number of polygons: %d\n",l_qty);
                p_object->polygon = (Triangle*)malloc(sizeof(Triangle) * l_qty);
                for (i=0; i<l_qty; i++)
                {

					fread (&temp_vertex[0], sizeof (unsigned short), 1, l_file);
					fread (&temp_vertex[1], sizeof (unsigned short), 1, l_file);
					fread (&temp_vertex[2], sizeof (unsigned short), 1, l_file);
                    sort(temp_vertex, temp_vertex+3);

                    p_object->polygon[i].a = temp_vertex[0];
                    p_object->polygon[i].b = temp_vertex[1];
                    p_object->polygon[i].c = temp_vertex[2];

					//printf("Polygon point c: %d\n",p_object->polygon[i].c);
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
					//printf("Face flags: %x\n",l_face_flags);
				}
				obj_list.push_back(make_pair(string(name), p_object));
                break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points)
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
//			case 0x4140:
//				fread (&l_qty, sizeof (unsigned short), 1, l_file);
//				for (i=0; i<l_qty; i++)
//				{
//					fread (&p_object->mapcoord[i].u, sizeof (float), 1, l_file);
//					printf("Mapping list u: %f\n",p_object->mapcoord[i].u);
//                    fread (&p_object->mapcoord[i].v, sizeof (float), 1, l_file);
//					printf("Mapping list v: %f\n",p_object->mapcoord[i].v);
//				}
//                break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
			default:
				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        }
	}
	fclose (l_file); // Closes the file stream
    return obj_list;
}
