#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


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
const char* fShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";


//helps us with resizing glfwfamecallback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//cameraPos camera front and camera up help us define the space we
//will move around based on these cordingates we also
//have camera up 
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//helps adjust rate at which camera moves
float cameraSpeed = 0.01f;

//fov set to whatever the user wants 
float fov          = 90.0f;
float screenWidth = 800;
float screenHeight = 800;
float aspectRatio  = (screenWidth / screenHeight);

//this helps us get depth perception. nearplane is the closest we will render verticies and farplane
//is the farthest
float nearPlane    = 0.1f;
float farPlane     = 100.0f;

//handles excape key and wasd
void getInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // movement along the plane with WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //we travel in the z direction positively by multiplying cameraspeed by the direction of our camera and adding
        //it back to pos
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //here we take the cross product of camerafront and camera up, we then add it to camera pos and 
    //the normilization is to smooth it out. this gives us a vector perpendicular to camera front and up which is
    //to the left
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


int main()
{
    //initialize GLFW
    glfwInit();
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // CORE GLFW profile means we have mot recent functions only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //error handling for window
    
    //verticies of our Sphere in 3d space normalized cords
    OBJdata data = parseOBJ();

    std::vector<GLfloat> verticies = data.finalVerticies;
    //std::vector<GLfloat> finalVerticies = data.finalVerticies;
    



    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //ensures that glfw associates the window we are using with the commands we give it
    glfwMakeContextCurrent(window);
    //Glad error handling
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }




    glDisable(GL_CULL_FACE);
    glViewport(0, 0, screenWidth, screenHeight);
    int indicies = verticies.size()/3;
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

    //need to create a vertex buffer to store all of these things
    //before we send them to the GPU
    //Create a Vertex buffer from gluint
    GLuint VAO, VBO;
    
    //glm::mat4 projection = glm::ortho(.5f,.5f,,,);
    glGenVertexArrays(1, &VAO);
    //Gen the buffer and bind it to the adress of the vbo
    glGenBuffers(1, &VBO);
    //binding vertex array
    glBindVertexArray(VAO);
    //binding buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //actually creating the buffer and telling it how many verticies are in it.
    // GL_STATIC means that verticies are modified once and used many times
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);




    //STOP GO NO FURTHER FIGURE THIS OUT
    glBufferData(GL_ARRAY_BUFFER, verticies.size()*sizeof(GLfloat),verticies.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    
    
    
    
    //the VAO stores pointers to the different VBOS and allows you
    // to switch between different VBOS wihtout lag
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    //enables vertex attrib pointer
    glEnableVertexAttribArray(0);

    //these two lines unbind the buffer and vao to help protect out buffers and arrays
    //from being changed
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //main while loop


    while(!glfwWindowShouldClose(window))
    {
        //clear screen and change the color 
        glClearColor(1.0f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bool wireframe = false;

        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
        else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Solid mode

        getInput(window);

        //activate shader program that we built earlier
        glUseProgram(shaderApp);

        //matrix projections using the MVP translation pipeline, the model will put our
        //sphere from its own cordinates to a space that we will use, it starts at 0,0,0
        //the view matrix will use look at to orient the camera to the correct view, still
        // unsure how this works entirely.
        //projection uses fov in radians along with aspect ratio and our plane
        //cutoffs to help us define the 3d space we are in. it makes sure we have depth perspective.
        glm::mat4 model = glm::mat4(1.0f); 
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

        //here we are sending the matricies to the GPU using a pointer to where they start
        glUniformMatrix4fv(glGetUniformLocation(shaderApp, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderApp, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderApp, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


        //bind all of our VBOs inside our VAO tells open gl we are using this VAO
        glBindVertexArray(VAO);
        //tells opengl that we are using triangles 0 is the starting 
        //index of our vertices and that we have 3 of them

        //changed to fit sphere might have to change to actual number
        //of verticies



        glUseProgram(shaderApp);



        glDrawArrays(GL_TRIANGLES, 0, verticies.size()/3);
        //glDrawArrays(GL_POINTS, 0, verticies.size() / 3);


        //render here
        //
        //
        //swap buffers
        //make sure you swap the buffers, why not quite sure yet.
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
