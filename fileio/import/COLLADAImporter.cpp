//
// Created by dongmin on 18. 3. 23.
//

#include <fstream>
#include <iostream>
#include "COLLADAImporter.h"

using namespace rapidxml;
using namespace std;


COLLADAImporter::COLLADAImporter() {

}

COLLADAImporter::~COLLADAImporter() {

}

TriangleMesh *COLLADAImporter::import(const char *filePath) {
    TriangleMesh* triangleMesh = new TriangleMesh();

    xml_document<> doc;
    xml_node<> * root_node;
    ifstream pFile(filePath);

    vector<char> buffer((istreambuf_iterator<char>(pFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);
    root_node = doc.first_node("COLLADA");
    string collada_version = root_node->first_attribute("version")->value();

    for (xml_node<>* library_geometries_node = root_node->first_node("library_geometries") ; library_geometries_node;
         library_geometries_node = library_geometries_node->next_sibling("library_geometries")){
        for (xml_node<>* geometry_node = library_geometries_node->first_node("geometry") ; geometry_node; geometry_node = geometry_node->next_sibling("geometry")){
            vector<Triangle*> geometry_triangles;
            vector<Vertex*> geometry_vertices;

            string geometry_name = geometry_node->first_attribute("id")->value();

            xml_node<>* mesh_node = geometry_node->first_node("mesh");
            if (mesh_node == NULL) exit(-1);

            xml_node<>* triangles_node = mesh_node->first_node("triangles");
            if (triangles_node == NULL) continue; //lines

            xml_node<>* source_node = mesh_node->first_node("source");

            string position_id = this->queryAttributeValueInNodes(mesh_node->first_node("vertices"), "input", "semantic", "POSITION", "source");
            position_id.erase(0, 1); // remove '#'

            for (; source_node; source_node = source_node->next_sibling("source")){
                if (strcmp(source_node->first_attribute("id")->value(), position_id.c_str()) == 0) break;
            }

            // check technique_common node
            xml_node<>* technique_common_node = source_node->first_node("technique_common");
            xml_node<>* accessor_node = technique_common_node->first_node("accessor");
            assert(atoi(accessor_node->first_attribute("stride")->value()) == 3);
            for (xml_node<>* param_node = accessor_node->first_node("param"); param_node; param_node = param_node->next_sibling("param")){
                assert(strcmp(param_node->first_attribute("type")->value(), "float") == 0);
            }
            int num_of_vertices = atoi(accessor_node->first_attribute("count")->value());

            // get Vertex Coords
            xml_node<>* float_array_node = source_node->first_node("float_array");
            string float_array_str = float_array_node->value();
            std::vector<std::string> float_array_strings = split(float_array_str, ' ');
            int float_array_count=atoi(float_array_node->first_attribute("count")->value());
            for (int i = 0 ; i < float_array_count / 3; i++){
                Vertex* vertex = new Vertex(stof(float_array_strings[i * 3 + 0]), stof(float_array_strings[i * 3 + 1]), stof(float_array_strings[i * 3 + 2]));
                vertex->setIndex(triangleMesh->vertices.size() + i);
                geometry_vertices.push_back(vertex);
                cout << vertex->toJSON() << endl;
            }
            assert(num_of_vertices == geometry_vertices.size());

            int num_of_triangles = atoi(triangles_node->first_attribute("count")->value());
            int num_of_input_nodes = 0, vertex_offset;
            for (xml_node<>* input_node = triangles_node->first_node("input") ; input_node ; input_node = input_node->next_sibling("input")){
                num_of_input_nodes++;
            }
            vertex_offset = atoi(this->queryAttributeValueInNodes(triangles_node, "input", "semantic", "VERTEX", "offset").c_str());

            string p = triangles_node->first_node("p")->value();
            std::vector<std::string> triangle_index_string = split(p, ' ');
            printf("%d\n", triangle_index_string.size());

            for (int i = 0 ; i < num_of_triangles; i++){
                int a_index = i * 3 * (num_of_input_nodes) + (num_of_input_nodes) * 0 + vertex_offset;
                int b_index = i * 3 * (num_of_input_nodes) + (num_of_input_nodes) * 1 + vertex_offset;
                int c_index = i * 3 * (num_of_input_nodes) + (num_of_input_nodes) * 2 + vertex_offset;
                int a = atoi(triangle_index_string[a_index].c_str());
                int b = atoi(triangle_index_string[b_index].c_str());
                int c = atoi(triangle_index_string[c_index].c_str());
                Triangle* triangle = new Triangle(geometry_vertices[a], geometry_vertices[b], geometry_vertices[c]);
                geometry_triangles.push_back(triangle);
                //cout << triangle->toJSON("") << endl;
            }

            triangleMesh->triangles.push_back(make_pair(geometry_name, geometry_triangles));
            triangleMesh->vertices.insert(triangleMesh->vertices.end(), geometry_vertices.begin(), geometry_vertices.end());
        }

    }
    return triangleMesh;
}


string COLLADAImporter::queryAttributeValueInNodes(rapidxml::xml_node<> *pNode, const char *childNodeName,
                                                   const char *condAttributeName, const char *condAttributeValue,
                                                   const char *resultAttributeName) {
    for (xml_node<>* input_node = pNode->first_node(childNodeName) ; input_node ; input_node->next_sibling(childNodeName)){
        if (strcmp(input_node->first_attribute(condAttributeName)->value(),condAttributeValue) == 0){
            //printf("%s\n", input_node->first_attribute("source")->value());
            string id(input_node->first_attribute(resultAttributeName)->value());
            return id;
        }
    }
    cerr << "There is no attribute to satisfy query" << endl;
}
