#include "sphere.h"
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <sstream>



OBJdata parseOBJ() {
    OBJdata result;
    std::vector<GLfloat> verticies;
    std::vector<GLfloat> finalVerticies;
    std::ifstream file("sphere1.obj");

    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") {
                float x, y, z;
                ss >> x >> y >> z;
                //std::cout << x << std::endl;
                verticies.push_back(x);
                verticies.push_back(y);
                verticies.push_back(z);
            } else if (type == "f") {
                std::string v1, v2, v3;
                ss >> v1 >> v2 >> v3;
    
                for (const std::string& v : {v1, v2, v3}) {
                    size_t slash = v.find('/');
                    int index = std::stoi(v.substr(0, slash)) - 1;
                    int offset = index *3;
                    finalVerticies.push_back(verticies[offset]);
                    finalVerticies.push_back(verticies[offset + 1]);
                    finalVerticies.push_back(verticies[offset + 2]);
                }
            }
        }
        file.close();
    } else {
        std::cerr << "failed obj file" << std::endl;
    }
    for (size_t i = 0; i < finalVerticies.size(); ++i) {
        std::cout << finalVerticies[i] << " ";
        // Optional: print a newline every 3 floats (per vertex) for clarity
        if ((i + 1) % 3 == 0) {
            std::cout << std::endl;
        }
    }
    result.finalVerticies = finalVerticies;
    return result;
}