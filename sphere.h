#ifndef SPHERE_H
#define SPHERE_H
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

struct OBJdata {
      std::vector<GLfloat> verticies;
      std::vector<GLfloat> finalVerticies;
};
OBJdata parseOBJ();


#endif