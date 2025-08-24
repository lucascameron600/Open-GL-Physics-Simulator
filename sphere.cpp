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
//generates all the spheres that we will use in our simulation
std::vector<Sphere> genSpheres(){
    std::vector<Sphere> spheres;

    for (int i = 0; i < 5; ++i) {
        //create sphere
        Sphere sphere;
        //sets original position of sphere right away
        sphere.spherePos = glm::vec3(i * 2.0f, 5.0f, 0.0f); 
        //initialize previous pos do not forget this line trust me
        sphere.prevPos = sphere.spherePos - glm::vec3(0.0f, 0.01f, 0.0f);
        // reset velocity to zero before pushing
        sphere.sphereVelocity = glm::vec3(0.0f);            
        spheres.push_back(sphere);

    }
    return spheres;
}
// this 
void RenderSpheres(GLuint shaderProgram, std::vector<Sphere>& spheres, int vertexCount) {
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


void Sphere::setSphereColor(glm::vec4& color) {
    sphereColor = color;  
}

void Sphere::updatePhysics(float deltaTime){
    //temporary vertex to store sphere pos
    glm::vec3 temporary = spherePos;
    //main impplementation of verlet integration(predicting next step
    //based on motion)
    spherePos = spherePos + (spherePos -prevPos) + acceleration * (deltaTime*deltaTime);
    //updates position of last frame
    prevPos = temporary;
    //makes sure the force from this fame doesent effect the next
    acceleration = glm::vec3(0.0f);
}

void Sphere::putForce(glm::vec3 force, float mass){
    acceleration += force/mass;
}
void Sphere::checkCollision(Sphere& opp){
    glm::vec3 delta = opp.spherePos - spherePos;
    float dist = glm::length(delta);
    float minDist = radius + opp.radius;

    if (dist < minDist && dist > 0.0f)
    {
        glm::vec3 correction = delta * ((minDist - dist) / dist * 0.5f);
        spherePos -= correction;
        opp.spherePos += correction;
    }
}
void Sphere::floorCollision(float floorY){
    if(spherePos.y - radius < floorY){
        glm::vec3 velocity = spherePos - prevPos;
        spherePos.y = floorY + radius;
        velocity.y *= -1;
        prevPos = spherePos - velocity;

    }
}


void Sphere::boundaryCollision(float boundaryMinx, float boundaryMaxx, float boundaryMiny, float boundaryMaxy, float boundaryMinz, float boundaryMaxz){
        glm::vec3& pos = spherePos;
        glm::vec3& vel = sphereVelocity;
        float radius = 1.0f; // Approximate radius of your sphere
    
        
        if (pos.x - radius < boundaryMinx) {
            pos.x = boundaryMinx + radius;
            vel.x *= -1.0f;
        } else if (pos.x + radius > boundaryMaxx) {
            pos.x = boundaryMaxx - radius;
            vel.x *= -1.0f;
        }
    
        
        if (pos.y - radius < boundaryMiny) {
            pos.y = boundaryMiny + radius;
            vel.y *= -1.0f;
        } else if (pos.y + radius > boundaryMaxy) {
            pos.y = boundaryMaxy - radius;
            vel.y *= -1.0f;
        }
    
    
        if (pos.z - radius < boundaryMinz) {
            pos.z = boundaryMinz + radius;
            vel.z *= -1.0f;
        } else if (pos.z + radius > boundaryMaxz) {
            pos.z = boundaryMaxz - radius;
            vel.z *= -1.0f;
        }
    }