
#ifndef SHADER_H
#define SHADER_H
#include "sphere.h"
#include <vector>       
#include <glad/gl.h>      
#include <glm/glm.hpp>  
#include <glm/gtc/type_ptr.hpp>
GLuint compileShaderProg();

class Render{
    public:
    Render();
    ~Render();

    void init(std::vector<GLfloat>& sphereVerticies, GLfloat* floorV,int floorV_size, int sphereVerticies_size);
    GLuint compileShaderProg();
    void renderFloor();
    void renderSpheres(std::vector<Sphere>& spheres);
    GLuint getShaderApp();
    void setSphereVertexCount(int count);
    void setFloorVertexCount(int count);
    

    private:
        GLuint VAO, VBO;
        GLuint shaderApp;
        int sphereVertexCount;
        int floorVertexCount;

};


#endif
