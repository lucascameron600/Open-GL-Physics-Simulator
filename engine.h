#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include <glm/glm.hpp>
#include "sphere.h"


class Engine{
    public:
    Engine();

    void updatePhysics(Sphere& sphere, float deltaTime);
    void putForce(Sphere& sphere, glm::vec3 force, float mass);
    void checkCollision(Sphere& sphere1, Sphere& sphere2);
    void floorCollision(Sphere& sphere, float floorY = 0.0f);
    void boundaryCollision(Sphere& sphere, float boundaryMinx, float boundaryMaxx, float boundaryMiny, float boundaryMaxy, float boundaryMinz, float boundaryMaxz);

};
#endif