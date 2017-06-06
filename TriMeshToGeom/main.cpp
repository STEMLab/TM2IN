//
//  main.cpp
//  TriMeshToGeom
//
//  Created by Dong's MacBook Pro on 2017. 6. 6..
//  Copyright © 2017년 Dong's MacBook Pro. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

typedef struct{
    float x;
    float y;
    float z;
} Vertex;

int main(int argc, const char * argv[]) {
    // insert code here...
    ifstream trin;
    trin.open("/Users/dong/Documents/dev/TriMeshToGeom/jongro/jongro.3DS");
    
    std::cout << "Hello, World!\n";
    return 0;
}
