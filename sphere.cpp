#include "sphere.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>



std::vector<float> parseOBJ() {
    std::vector<float> vertices;
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
                std::cout << x << std::endl;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
        file.close();
    } else {
        std::cerr << "failed obj file" << std::endl;
    }

    return vertices;
}