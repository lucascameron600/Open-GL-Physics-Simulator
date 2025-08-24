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
    //needed for verlet integration
    glm::vec3 prevPos = spherePos;
    // Acceleration vector (resets each frame)
    glm::vec3 acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
    //initial velocity before applied gravity is 0
    glm::vec3 sphereVelocity = glm::vec3(0.0f);
    float radius = 1.0f; 
    glm::vec4 sphereColor = glm::vec4(1.0f,0.0f,0.0f,1.0f);


    //void setSphereColor(glm::vec4& color);
    //void updatePhysics(float deltaTime);
    //void putForce(glm::vec3 force, float mass);
    //void checkCollision(Sphere& opp);
    //void floorCollision(float floorY = 0.0f);
    //void boundaryCollision(float boundaryMinx, float boundaryMaxx, float boundaryMiny, float boundaryMaxy, float boundaryMinz, float boundaryMaxz);

};

//class Sphere {
//    private:
//        glm::vec3 position;
//        glm::vec3 prevPosition;
//        glm::vec3 acceleration;
//        glm::vec3 velocity;
//        float radius;
//        glm::vec4 color;
//    
//    public:
//        Sphere(glm::vec3 pos = glm::vec3(0.0f), float r = 1.0f);
//    
//        void applyForce(const glm::vec3& force, float mass);
//        void updatePhysics(float deltaTime);
//        void checkCollision(Sphere& other);
//        void floorCollision(float floorY);
//        void boundaryCollision(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
//    
//        // Getter methods (no direct access to position, etc.)
//        glm::vec3 getPosition() const { return position; }
//        float getRadius() const { return radius; }
//    
//        // Setter methods if needed (keep control!)
//        void setColor(const glm::vec4& col) { color = col; }
//        glm::vec4 getColor() const { return color; }
//    };

std::vector<Sphere> genSpheres(int numSpheres);
void addSpheres(std::vector<Sphere>& spheres, int count);
void renderSpheres(GLuint shaderProgram, std::vector<Sphere>& spheres, int vertexCount);
std::vector<GLfloat> parseOBJ();

#endif