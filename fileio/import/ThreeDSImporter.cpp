
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

vector<TriangleMesh*> ThreeDSImporter::import(const char *p_filename)
{
	int i; //Index variable
	FILE *l_file; //File pointer
    char name[BUFSIZ];

	unsigned short l_chunk_id; //Chunk identifier
	unsigned int l_chunk_length; //Chunk lenght

	unsigned char l_char; //Char variable
	unsigned short l_qty; //Number of elements in each chunk

	unsigned short l_face_flags; //Flag that stores some face information

    assert( (l_file=fopen (p_filename, "rb") ) != NULL);

    vector<TriangleMesh*> meshList;
    TriangleMesh* currentMesh = new TriangleMesh();

    string group_name;
    int f_count =0, v_count = 0;

    while (ftell (l_file) < this->filelength (fileno (l_file))) //Loop to scan the whole file
	{
		//getch(); //Insert this command for debug (to wait for keypress for each chuck reading)
		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
		fread (&l_chunk_length, 4, 1, l_file); //Read the length of the chunk

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
					fread (&l_char, 1, 1, l_file);
                    name[i] = l_char;
                    i++;
                }while(l_char != '\0');
                currentMesh->name = name;

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
                fseek(l_file, l_chunk_length-6, SEEK_CUR);
        }
	}
    if (f_count != 0){
        meshList.push_back(currentMesh);
    }
	fclose (l_file); // Closes the file stream
    return meshList;
}
