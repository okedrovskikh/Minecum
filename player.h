#pragma once
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "block.h"
#include "chunk.h"
#include <limits>
#include <vector>

const float WIDTH_X = 0.6f;
const float HEIGHT_Y = 0.6f;
const float WIDTH_Z = 0.6f;

enum State {
    STANDING, MIDAIR, SWIMING, FLYING
};

class Player
{
public:
    glm::vec3 position;
	Camera camera;
	Player();
    Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO);
    void processMovement(GLFWwindow* window, float deltaTime, Chunk v);
    bool AABB(float one, float two);
    ~Player();
private:
    float yVelocity = 0.0f;
    State state;
    float vertices[288] = {
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    void applyMotion(glm::vec3 motion, Chunk v);
};