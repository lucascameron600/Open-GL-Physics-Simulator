#include "sphere.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>



std::vector<Vertex> parseOBJ(){

    std::vector<Vertex> v;
    std::ifstream file("sphere.obj");
    std::string line;

    if(file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if(type == "v"){
                Vertex v;
                ss >> v.x >> v.y >> v.z;
                v.push_back(v);
            }
        }
    file.close();
} else{//handle file opeining error
    }

return v
}