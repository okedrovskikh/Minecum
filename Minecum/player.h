#pragma once
#include <limits>
#include <vector>

#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "world.h"

const float PLAYER_SIZE_X = 0.6f;
const float PLAYER_SIZE_Y = 0.6f;
const float PLAYER_SIZE_Z = 0.6f;

const float INTERACTION_RADIUS = 3.0f;
const float MOVEMENT_SPEED = 3.0f;

const float fMax = std::numeric_limits<float>::max();

enum State {
    STANDING, MIDAIR, SWIMING, FLYING
};

class Player
{
public:
    unsigned int VAO;
    unsigned int VBO;
    glm::vec3 position;
	Camera camera;
    Shader* shader;
    Texture* texture;
    BlockType chosenBlock;
	Player();
    Player(glm::vec3 position, std::string vertexPath, std::string fragmentPath, std::string texturePath);
    void processMovement(GLFWwindow* window, float deltaTime, const World& world);
    void rayCast(const World& world);
    void processLeftClick(const World& world);
    void processRightClick(const World& world);
    void updateCamera();
    ~Player();
private:
    float yVelocity;
    State state;
    int interactionChunkIndex;
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
    void applyMotion(glm::vec3 motion, const std::vector<Chunk*>& chunks);
    bool axisCollision(float one, float two, float parameter = 0.3f);
    int* getOrder();
};

int sgn(float a);
