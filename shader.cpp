#include <iostream>
#include <glad/gl.h>
#include "shader.h"

//depricated vertex shader
////vertex shader source code
//const char* vShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec4 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"

//////////////////////////////////////////////////////////////////
//inside this shader, i have added model view and projection matrixes
//in order to accept the data we will send to the gpu
//we then perform matrix multiplication on them to perform the nessacary
//transformations

const char* vShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
        
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
        
    void main()
    {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    )glsl";
    
    
    //Fragment Shader source code
    const char* fShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
        
    uniform vec4 inputColor;
        
    void main()
    {
        FragColor = inputColor;
    }
    )glsl";
    

GLuint compileShaderProg(){
    //unisgned integer shader object v shader is the name
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    //we must attach the shader 1 means number of strings in the array
    //vshdadersource points to the source code definded above
    //Null means the source string is null at the end ???
    glShaderSource(vShader, 1 , &vShaderSource, NULL);
    //compile shader
    glCompileShader(vShader);
    //exact same steps from above but here
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1 , &fShaderSource, NULL);
    glCompileShader(fShader);
    //generating a shader program to link everything
    GLuint shaderApp = glCreateProgram();
    //attaching vshader and fshader to the app
    glAttachShader(shaderApp, vShader);
    glAttachShader(shaderApp, fShader);
    //linking the program????
    glLinkProgram(shaderApp);
    //deleting shaders after we are done with them because they are already linked to the program
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return shaderApp;
}