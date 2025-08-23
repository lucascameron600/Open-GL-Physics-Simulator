#ifndef SPHERE_H
#define SPHERE_H
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Sphere

{
    //set initial sphere postition, 5 above y cord
    glm::vec3 spherePos = glm::vec3(0.0f,5.0f,0.0f);
    //initial velocity before applied gravity is 0
    glm::vec3 sphereVelocity = glm::vec3(0.0f);


};

struct OBJdata {
      std::vector<GLfloat> verticies;
      std::vector<GLfloat> finalVerticies;
};
OBJdata parseOBJ();


#endif