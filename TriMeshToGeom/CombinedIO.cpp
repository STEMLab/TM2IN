#include "CombinedIO.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Kernel/global_functions.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Plane_3 Plane_3;

int CombinedIO:: exportBinary(ofstream& fout, vector<TriangleSpace*>& ts){
    ll size = ts.size();
    fout.write((char*)&size, sizeof(size));
    for (int i = 0 ; i < size ; i++){
        exportBinary(fout, ts[i]->polygon_list);
    }
    return 0;
}

int CombinedIO::exportBinary(ofstream& fout, std::vector<CombinedPolygon*>& polygon_list){
    ll size = polygon_list.size();
    fout.write((char*)&size, sizeof(size));
    for (unsigned int i = 0; i < size ; i++){
        exportBinary(fout, polygon_list[i]->v_list);
        Vector_3 normal = polygon_list[i]->av_normal;
        fout.write((char*)(&normal.x()), sizeof(normal.x()));
        fout.write((char*)(&normal.y()), sizeof(normal.y()));
        fout.write((char*)(&normal.z()), sizeof(normal.z()));
    }
    return 0;
}

int CombinedIO::exportBinary(ofstream& fout, std::vector<Vertex*>& v_list){
    ll size = v_list.size();
    fout.write((char*)&size, sizeof(size));
    for (unsigned int i = 0; i < size ; i++){
        fout.write((char*)&v_list[i]->index, sizeof(ll));
    }
    return 0;
}

int CombinedIO::importBinary(ifstream& fin, OBJCollection* objcl){
    ll size;
    fin.read((char*)&size, sizeof(size));
    for (int i = 0 ; i < size ; i++){
        CombinedIO::importBinary(fin, objcl->vertex, objcl->space_list[i]);
    }
    return 0;
}

int CombinedIO::importBinary(ifstream& fin, vector<Vertex*>& vertex, TriangleSpace* ts){
    ll size;
    fin.read((char*)&size, sizeof(size));
    for (int i = 0 ; i < size ; i++){
        CombinedPolygon* cp = new CombinedPolygon();
        importBinary(fin, vertex, cp);
        ts->polygon_list.push_back(cp);
    }
    return 0;
}

int CombinedIO::importBinary(ifstream& fin, vector<Vertex*>& vertex, CombinedPolygon* cp){
    ll size;
    fin.read((char*)&size, sizeof(size));
    for (int i = 0 ; i < size; i++){
        ll index;
        fin.read((char*)&index, sizeof(ll));
        cp->v_list.push_back(vertex[index]);
    }

    double x,y,z;
    fin.read((char*)&x, sizeof(double));
    fin.read((char*)&y, sizeof(double));
    fin.read((char*)&z, sizeof(double));

    cp->av_normal = Vector_3(x,y,z);
    return 0;
}