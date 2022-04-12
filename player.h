#pragma once
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "world.h"
#include <limits>
#include <vector>

const float PLAYER_SIZE_X = 0.6f;
const float PLAYER_SIZE_Y = 0.6f;
const float PLAYER_SIZE_Z = 0.6f;

const float INTERACTION_RADIUS = 3.0f;

enum State {
    STANDING, MIDAIR, SWIMING, FLYING
};

class Player
{
public:
    glm::vec3 position;
	Camera camera;
    Shader shader;
    Texture texture;
	Player();
    Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO, const Shader& shader, const Texture& texture);
    void processMovement(GLFWwindow* window, float deltaTime, const Chunk& chunk);
    void rayCast(Chunk& chunk);
    void processLeftClick(Chunk& chunk);
    void processRightClick(Chunk& chunk);
    void updateCamera();
    ~Player();
private:
    float yVelocity;
    State state;
    int interactionBlockIndex;
    float vertices[288] = {
        -0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f,  0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    void applyMotion(glm::vec3 motion, const Chunk& chunk);
    bool axisCollision(float one, float two, float parameter = 0.3f);
    std::vector<int> getOrder();
};

int sgn(float a);
