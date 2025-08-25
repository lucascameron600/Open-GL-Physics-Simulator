#include "sphere.h"
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <sstream>




std::vector<GLfloat> parseOBJ() {
    
    std::vector<GLfloat> verticies;
    std::vector<GLfloat> finalVerticies;
    std::ifstream file("sphere2.obj");

    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream stream(line);
            std::string type;
            stream >> type;

            if (type == "v") {
                float x, y, z;
                stream >> x >> y >> z;

                verticies.push_back(x);
                verticies.push_back(y);
                verticies.push_back(z);

            } else if (type == "f") {
                std::string v1, v2, v3;
                stream >> v1 >> v2 >> v3;
    
                for (const std::string& v : {v1, v2, v3}) {
                    int index = std::stoi(v.substr(0, v.find('/'))) - 1;
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

    return finalVerticies;
}

void renderSpheres(GLuint shaderProgram, std::vector<Sphere>& spheres, int vertexCount) {
    for (Sphere& sphere : spheres) {
        //matrix transformation by multiplying shere pos with identity matrix sets that to model we will send to shader
        glm::mat4 model = glm::translate(glm::mat4(1.0f), sphere.spherePos);
        //what part of the shader we are using and a pointer to it
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //sets color for shader
        glUniform4f(glGetUniformLocation(shaderProgram, "inputColor"), 0.1f, 0.9f, 0.1f, 1.0f);
        //draws
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
}


