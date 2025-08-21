#include <iostream>


#include <glad/gl.h>
#include <GLFW/glfw3.h>
//vertex shader source code
const char* vShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

//handles excape key
void getInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

    //verticies of our traingle in 3d space normalized cords
    GLfloat verticies[] =
    {
        -0.75f, -0.75f,0.0f,
        0.75f, -0.75f,0.0f,
        0.0f, 0.75f, 0.0f

    };


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    

    glViewport(0, 0, 800, 600);

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
    
    glGenVertexArrays(1, &VAO);
    //Gen the buffer and bind it to the adress of the vbo
    glGenBuffers(1, &VBO);
    //binding vertex array
    glBindVertexArray(VAO);
    //binding buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //actually creating the buffer and telling it how many verticies are in it.
    // GL_STATIC means that verticies are modified once and used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
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
        getInput(window);

        //activate shader program that we built earlier
        glUseProgram(shaderApp);
        //bind all of our VBOs inside our VAO tells open gl we are using this VAO
        glBindVertexArray(VAO);
        //tells opengl that we are using triangles 0 is the starting 
        //index of our vertices and that we have 3 of them
        glDrawArrays(GL_TRIANGLES, 0, 3);
        


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
