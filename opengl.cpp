#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "engine.h"
#include "sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


//helps us with resizing glfwfamecallback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
float boundaryMaxx = 50.0f;
float boundaryMinx = -50.0f;

float boundaryMaxy = 50.0f;
float boundaryMiny = 0.0f;

float boundaryMaxz = 50.0f;
float boundaryMinz = -50.0f;

GLfloat floorV[] = {
    
    -50.0f, 0.0f, -50.0f,
     50.0f, 0.0f, -50.0f,
     50.0f, 0.0f,  50.0f,

    -50.0f, 0.0f, -50.0f,
     50.0f, 0.0f,  50.0f,
    -50.0f, 0.0f,  50.0f
};


//cameraPos camera front and camera up help us define the space we
//will move around based on these cordingates we also
//have camera up 
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//////////////////////////////////////////////////////////
/////////////GLOBALCONSTANTS////////////////////////
////////////////////////////////
//helps adjust rate at which camera moves
float cameraSpeed = 10.0f;

//fov set to whatever the user wants 
float fov          = 90.0f;
float screenWidth = 800;
float screenHeight = 800;
float aspectRatio  = (screenWidth / screenHeight);
//acceleration constant for gravity
float gravity = -9.85;

//this helps us get depth perception. nearplane is the closest we will render verticies and farplane
//is the farthest
float nearPlane    = 0.1f;
float farPlane     = 100.0f;
//handle time
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//for generating spheres
bool bKeyPressed = false;
//handles excape key and wasd
void getInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // movement along the plane with WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //we travel in the z direction positively by multiplying cameraspeed by the direction of our camera and adding
        //it back to pos
        cameraPos += cameraSpeed * cameraFront * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //here we take the cross product of camerafront and camera up, we then add it to camera pos and 
    //the normilization is to smooth it out. this gives us a vector perpendicular to camera front and up which is
    //to the left
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed* deltaTime;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed * deltaTime;

}


int main()
{
    Engine Engine;
    int firstSpheres = 5;
    int addBalls = 1;
    std::vector<Sphere> spheres = Engine.genSpheres(firstSpheres);

    Sphere firstsphere;



    //initialize GLFW
    glfwInit();
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // CORE GLFW profile means we have mot recent functions only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //error handling for window


    
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Spheres!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //ensures that glfw associates the window we are using with the commands we give it
    glfwMakeContextCurrent(window);

    //verticies of our Sphere in 3d space normalized cords
    

    std::vector<GLfloat> verticies = parseOBJ();
    std::vector<GLfloat> combinedVerticies = verticies;
    combinedVerticies.insert(combinedVerticies.end(), std::begin(floorV), std::end(floorV));
    int sphereVertexCount = verticies.size() / 3;
    int floorVertexCount = sizeof(floorV) / sizeof(GLfloat) / 3;

    //std::vector<GLfloat> finalVerticies = data.finalVerticies;

    //Glad error handling
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glDisable(GL_CULL_FACE);
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);
/////////////////////////////////////////////////////////////
///////////////SHADERS//////////////////////////
    GLuint shaderApp = compileShaderProg();
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
    //giving buffer data the combined verticies becuase we are drawring all of
    //them in one VBO
    glBufferData(GL_ARRAY_BUFFER, combinedVerticies.size()*sizeof(GLfloat),combinedVerticies.data(), GL_STATIC_DRAW);
    //single traingle buffer data
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


    //main while loop

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& imio = ImGui::GetIO(); (void) imio;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //track delta time accumulation
    float physicsAccumulator = 0.0f;
    while(!glfwWindowShouldClose(window))
    {

        //get getting delta time in order to update pysics based
        //on time passed reguardless of framerate
        //helps us get smooth motion between frames
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //add to physics accumulator every frame, this helps us only move at a fixed delta time//

        physicsAccumulator += deltaTime;

        // FPS counter
        static float timer = 0.0f;
        static int frames = 0;

        frames++;
        timer += deltaTime;




        //clear screen and change the color 
        glClearColor(1.0f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::Text("GUI");
        ImGui::SliderFloat("Camera Speed", &cameraSpeed, 1.0f, 100.0f);
        ImGui::SliderFloat("Gravity", &gravity, 0.0f, -30.0f);
        ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f);
        ImGui::SliderInt("Add Balls", &addBalls, 1, 20);
        ImGui::Text("Sphere Count: %zu", spheres.size());
        ImGui::Text("Sphere Y Position: %.2f", spheres[0].spherePos.y);
        ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
        ImGui::End();

        //Change between a wireframe fill and full fill helps
        //to see the 3d component without lighting
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
        //the view matrix will use look at to orient the camera to the correct view.
        //projection uses fov in radians along with aspect ratio and our plane
        //cutoffs to help us define the 3d space we are in. it makes sure we have depth perspective.
        //this was before gravity//glm::mat4 model =       glm::mat4(1.0f); 
        //glm::mat4 model =       glm::translate(glm::mat4(1.0f), firstsphere.spherePos);
        glm::mat4 view =        glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection =  glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        //here we are sending the matricies to the GPU using a pointer to where they start
        //sglUniformMatrix4fv(glGetUniformLocation(shaderApp, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderApp, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderApp, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        //bind all of our VBOs inside our VAO tells open gl we are using this VAO
        glBindVertexArray(VAO);
        //tells opengl that we are using triangles 0 is the starting 
        //index of our vertices and that we have 3 of them

        //////////////////////////////////////////////////////
        //draw floor then sphere
        ////////////////////////////
        glm::mat4 floormodel = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderApp, "model"), 1, GL_FALSE, glm::value_ptr(floormodel));
        //setting color for frament shader to color floor
        glUniform4f(glGetUniformLocation(shaderApp, "inputColor"), 0.5f, 0.5f, 0.4f, 1.0f);
        glDrawArrays(GL_TRIANGLES, sphereVertexCount, floorVertexCount);



        //render here
        //make sure you swap the buffers!!!!

        renderSpheres(shaderApp, spheres, sphereVertexCount);


    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        if (!bKeyPressed) {
            Engine.addSpheres(spheres, addBalls);
            bKeyPressed = true;
        }
    } else {
        bKeyPressed = false;
    }
        const float fixedDeltaTime = 0.016f;

        while(physicsAccumulator >= fixedDeltaTime){
        
        for(Sphere& sphere : spheres){
            Engine.putForce(sphere, glm::vec3(0.0f, gravity, 0.0f), 0.1f);
        }

        for(Sphere& sphere : spheres){
            Engine.updatePhysics(sphere, fixedDeltaTime);
        }
        for (size_t i = 0; i < spheres.size(); ++i) {
            for (size_t j = i + 1; j < spheres.size(); ++j) {
            Engine.checkCollision(spheres[i], spheres[j]);
            }
        }
        for(Sphere& sphere : spheres){
            Engine.floorCollision(sphere, 0.0f);
        }
        for(Sphere& sphere : spheres){
            Engine.boundaryCollision(sphere, boundaryMinx, boundaryMaxx, boundaryMiny, boundaryMaxy, boundaryMinz, boundaryMaxz);
        }
        physicsAccumulator -= fixedDeltaTime;
    }

//        for (Sphere& sphere : spheres) {
//            glm::mat4 model = glm::translate(glm::mat4(1.0f), sphere.spherePos);
//            glUniformMatrix4fv(glGetUniformLocation(shaderApp, "model"), 1, GL_FALSE, glm::value_ptr(model));
//        
//            glUniform4f(glGetUniformLocation(shaderApp, "inputColor"), 0.1f, 0.9f, 0.1f, 1.0f); // Optional: vary per-sphere
//            glDrawArrays(GL_TRIANGLES, 0, sphereVertexCount);
//        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //clean up vertex buffer and vertex array on gpu after all work is done
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderApp);




    glfwTerminate();
    return 0;
}
