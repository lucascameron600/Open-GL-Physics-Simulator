#include <iostream>
#include <glad/gl.h>
#include <string>
#include <GLFW/glfw3.h>
#include "engine.h"
#include "sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


//helps us with resizing glfwfamecallback

float boundaryMaxx = 50.0f;
float boundaryMinx = -50.0f;

float boundaryMaxy = 50.0f;
float boundaryMiny = 0.0f;

float boundaryMaxz = 50.0f;
float boundaryMinz = -50.0f;

Render Render;
Engine Engine;

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
float aspectRatio  = (screenWidth/screenHeight);
//acceleration constant for gravity
float gravity = -9.85;

//this helps us get depth perception. nearplane is the closest we will render verticies and farplane
//is the farthest
float nearPlane    = 0.1f;
float farPlane     = 100.0f;

//handle time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//handles excape key and wasd
bool bKeyPressed = false;

//track delta time accumulation
float physicsAccumulator = 0.0f;
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

    int firstSpheres = 5;
    int addBalls = 1;
    std::vector<Sphere> spheres = Engine.genSpheres(firstSpheres);

    Sphere firstsphere;


    std::vector<GLfloat> verticies = parseOBJ();
    std::vector<GLfloat> combinedVerticies = verticies;
    combinedVerticies.insert(combinedVerticies.end(), std::begin(floorV), std::end(floorV));

    GLFWwindow* window = Render.glfwSetup(screenWidth, screenHeight, "Spheres!");

    
    Render.init(verticies, floorV, sizeof(floorV)/sizeof(GLfloat), verticies.size());



    //main while loop

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& imio = ImGui::GetIO(); (void) imio;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    

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
        GLuint shaderApp = Render.getShaderApp();
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


        //render here
        //make sure you swap the buffers!!!!
        Render.renderFloor();
        Render.renderSpheres(spheres);


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

    ////clean up vertex buffer and vertex array on gpu after all work is done
    Render.cleanUp(); 



    glfwTerminate();
    return 0;
}
