
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>

#include "ThreeDSImporter.h"

using namespace std;

ThreeDSImporter::ThreeDSImporter()
{
    //ctor
}

ThreeDSImporter::~ThreeDSImporter()
{
    //dtor
}

long ThreeDSImporter::filelength(int f)
{
	struct stat buf;

	fstat(f, &buf);

	return(buf.st_size);
}

TriangleMesh* ThreeDSImporter::import(const char *p_filename)
{
	int i; //Index variable
	FILE *l_file; //File pointer
    char name[BUFSIZ];

	unsigned short l_chunk_id; //Chunk identifier
	unsigned int l_chunk_length; //Chunk lenght

	unsigned char l_char; //Char variable
	unsigned short l_qty; //Number of elements in each chunk

	unsigned short l_face_flags; //Flag that stores some face information

    if ( (l_file=fopen (p_filename, "rb") ) == NULL) return NULL;

    TriangleMesh* triangleMesh = new TriangleMesh();
    vector<Triangle*> triangles;

    string group_name;
    int f_count =0, v_count = 0;
    int groupVertexInit = 0;

    while (ftell (l_file) < this->filelength (fileno (l_file))) //Loop to scan the whole file
	{
		//getch(); //Insert this command for debug (to wait for keypress for each chuck reading)
		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
		fread (&l_chunk_length, 4, 1, l_file); //Read the lenght of the chunk

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
                if (f_count != 0){
                    triangleMesh->triangles.push_back(make_pair(group_name, triangles));
                    triangles.clear();
                    f_count = 0;
                    groupVertexInit = v_count;
                }
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    name[i] = l_char;
                    i++;
                }while(l_char != '\0' && i<20);
                group_name = name;

                if (strstr(name, "FC")){
                    cout << "skip furniture" << endl;
                    fseek(l_file, l_chunk_length -i -6, SEEK_CUR);
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
			//             + 3 x float (vertices coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
			case 0x4110:
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                printf("Number of vertices: %d\n",l_qty);

                for (i=0; i<l_qty; i++)
                {
                    Vertex* vt = new Vertex();
                    float temp;
                    fread (&temp, sizeof(float), 1, l_file);
                    vt->setX(temp);
                    fread (&temp, sizeof(float), 1, l_file);
                    vt->setY(temp);
                    fread (&temp, sizeof(float), 1, l_file);
                    vt->setZ(temp);
                    vt->index = i + v_count;
                    triangleMesh->vertices.push_back(vt);
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

			case 0x4120:
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                printf("Number of triangles: %d\n",l_qty);
                f_count = l_qty;
                for (i=0; i < l_qty; i++)
                {
                    unsigned short a,b,c;
                    fread (&a, sizeof (unsigned short), 1, l_file);
                    fread (&b, sizeof (unsigned short), 1, l_file);
                    fread (&c, sizeof (unsigned short), 1, l_file);
                    fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
                    Vertex* va = triangleMesh->vertices[a + groupVertexInit];
                    Vertex* vb = triangleMesh->vertices[b + groupVertexInit];
                    Vertex* vc = triangleMesh->vertices[c + groupVertexInit];
                    Triangle* tri = new Triangle(va, vb, vc);
                    triangles.push_back(tri);
                }
                break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points)
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
			case 0x4140:
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                cout << "mappingcoords : " << l_qty << endl;
                for (i=0; i<l_qty; i++)
                {
                    float temp3;
                    fread (&temp3, sizeof (float), 1, l_file);
                    fread (&temp3, sizeof (float), 1, l_file);
                }
                break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
			default:
				 fseek(l_file, l_chunk_length-6, SEEK_CUR);
        }
	}
	fclose (l_file); // Closes the file stream
    return triangleMesh;
}
